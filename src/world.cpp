#include <world.h>
#include <misc.h>
#include <mymath.h>

#include <algorithm>
#include <proceduralGeneration.h>



ThreadPool::ThreadPool(int n_workers) : n_workers(n_workers), ioService(), thread_group(), work(ioService)
{
    for(int i=0; i<n_workers; i++){
        thread_group.create_thread(boost::bind(&boost::asio::io_service::run, &ioService));
    }
}

void ThreadPool::stop()
{   
    ioService.stop();
    thread_group.join_all();
}


World::World(Settings& settings, Camera& camera, std::unordered_map<std::string, unsigned int>& blockIDMap):
    settings(settings),
    chunk_manager(settings, chunk_map, blockIDMap),
    block_is_targeted(false),
    player(settings, camera),
    blockIDMap(blockIDMap)
{

}


void World::gameLogic(double delta_time)
{
    glm::vec3& player_pos = player.camera.pos;
    //print_vec3("player", player.camera.pos);
    chunk_manager.gen_nearby_chunks(player_pos);
    chunk_manager.add_new_chunks();
    chunk_manager.remove_far_chunks(player_pos);
    targetBlockRay(player_pos);
}


double rayProjectionHelperFun(double& x, double& dir)
{
    double len = x - std::floor(x);
    if(dir>0){
        if(len==0)
            len=1;
        else
            len = 1-len;
    }
    else{//dir_x>0 or 0, but then we return inf and thats fine, since all dir_xyz wont be 0
        if(len==0)
            len=-1;
        else
            len = -len;
    }
    return len/dir;
}
void World::targetBlockRay(glm::vec3 v, glm::vec3 enter_direction, int step){targetBlockRay(v.x, v.y, v.z, enter_direction, step);}
void World::targetBlockRay(double x, double y, double z, glm::vec3 enter_direction, int step) //warning for stack overflow segfault
{
    block_is_targeted = false;
    double dir_x = player.camera.front.x;
    double dir_y = player.camera.front.y;
    double dir_z = player.camera.front.z;
    if(player.distanceTo(x,y,z) < settings.reach)
    {
        if(step==0){
            target_pos = glm::vec3(std::floor(x), std::floor(y), std::floor(z));
        }
        else{
            target_pos += enter_direction;
        }
        if(isInBounds(target_pos))
        {
            unsigned int block = getBlock(target_pos);
            if(block != 0)
            {
                block_is_targeted = true;
                target_facing = -enter_direction;
                return;
            }
        }

        //projection on cube sides
        double tx = rayProjectionHelperFun(x, dir_x);
        double ty = rayProjectionHelperFun(y, dir_y);
        double tz = rayProjectionHelperFun(z, dir_z);
        if(tx<ty && tx<tz)
            targetBlockRay(std::nearbyint(x+tx*dir_x), y+tx*dir_y, z+tx*dir_z, glm::vec3(sign(dir_x),0,0), step+1);
        else if(ty<tx && ty<tz)
            targetBlockRay(x+ty*dir_x, std::nearbyint(y+ty*dir_y), z+ty*dir_z, glm::vec3(0,sign(dir_y),0), step+1);
        else
            targetBlockRay(x+tz*dir_x, y+tz*dir_y, std::nearbyint(z+tz*dir_z), glm::vec3(0,0,sign(dir_z)), step+1);
    }
}


void World::placeBlockOnCursor()
{
    if(block_is_targeted){
        setBlock(target_pos+target_facing, blockIDMap["Granite"]);
    }
}
void World::destroyBlockOnCursor()
{
    if(block_is_targeted){
        setBlock(target_pos, 0);
    }
}


void World::end()
{
    chunk_manager.end();
}


unsigned int& World::getBlock(glm::vec3 p){return chunk_manager.getBlock(p.x, p.y, p.z);}
unsigned int& World::getBlock(int x, int y, int z){return chunk_manager.getBlock(x, y, z);}
bool World::isInBounds(glm::vec3 p){return chunk_manager.isInBounds(p);}
bool World::isInBounds(int x, int y, int z){return chunk_manager.isInBounds(x, y, z);}
void World::setBlock(glm::vec3 v, int blockID){return chunk_manager.setBlock(v, blockID);}
void World::setBlock(int x, int y, int z, int blockID){return chunk_manager.setBlock(x,y,z, blockID);}


////////////////////////////////////
///////////CHUNKMANAGER/////////////
////////////////////////////////////


ChunkManager::ChunkManager(Settings& settings, ChunkMapivec2& chunk_map, std::unordered_map<std::string, unsigned int>& blockIDMap):
    settings(settings),
    chunk_map(chunk_map),
    blockIDMap(blockIDMap),
    octaves({{200, 50}, {100, 40}, {10, 3}}), //(size, ampl)
    n_threads(4),
    pool(n_threads),
    work_queue_size(0)
{
    sort_circle();
    for(int i=0; i<octaves.size(); i++){
        sizes.push_back(octaves[i].x);
        amplitudes.push_back(octaves[i].y);
    }
}


void ChunkManager::sort_circle() //in world coords
{
    // given R, make a square with sidelength 2R. then filter out dist. then sort dist
    int R = settings.getRenderDistance();
    circle_chunk_positions.clear();
    for(int x_r=-R; x_r<=R; x_r++)
    {
        for(int z_r=-R; z_r<=R; z_r++)
        {
            glm::ivec2 coord = glm::ivec2(x_r, z_r);
            if (R > glm::length(glm::vec2(coord)))
            {
                circle_chunk_positions.push_back(coord);
            }
        }
    } 
    //now sort based on distance from origin
    std::sort(circle_chunk_positions.begin(), circle_chunk_positions.end(), dist_from_origin);
}


std::pair<glm::ivec2, glm::vec3> ChunkManager::findChunkPos(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int chunk_x = std::floor(x / (float)ch_width);
    int chunk_z = std::floor(z / (float)ch_depth);
    int local_x = x - chunk_x*ch_width;
    int local_z = z - chunk_z*ch_depth;
    return std::pair<glm::ivec2, glm::vec3>(glm::ivec2(chunk_x, chunk_z), glm::vec3(local_x,y,local_z));
}
std::pair<glm::ivec2, glm::vec3> ChunkManager::findChunkPos(glm::vec3 v)
{
    return findChunkPos(std::floor(v.x), std::floor(v.y), std::floor(v.z));
}


unsigned int& ChunkManager::getBlock(int x, int y, int z)
{
    glm::ivec2 chunk_pos;
    glm::vec3 local_pos;
    std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);
    unsigned int& block = chunk_map.at(chunk_pos).getBlock(local_pos);
    return block;
}
unsigned int& ChunkManager::getBlock(glm::vec3 p)
{
    return getBlock(p.x, p.y, p.z);
}


bool ChunkManager::isInBounds(glm::vec3 v)
{
    return isInBounds(v.x, v.y, v.z);
}
bool ChunkManager::isInBounds(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();

    int chunk_x = std::floor(x / (float)ch_width);
    int chunk_z = std::floor(z / (float) ch_depth);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<ch_height)
        if(chunk_map.find(pos)!=chunk_map.end())
            return true;
    return false;
}


void ChunkManager::setBlock(glm::vec3 v, int blockID)
{
    setBlock(v.x, v.y, v.z, blockID);
}
void ChunkManager::setBlock(int x, int y, int z, int blockID)
{
    glm::ivec2 chunk_pos;
    glm::vec3 local_pos;
    std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);

    chunk_map.at(chunk_pos).setBlock(local_pos, blockID);
    updateVisible(x, y, z);
    updateVisible(x+1, y, z);
    updateVisible(x-1, y, z);
    updateVisible(x, y+1, z);
    updateVisible(x, y-1, z);
    updateVisible(x, y, z+1);
    updateVisible(x, y, z-1);
}

void ChunkManager::updateVisible(int x, int y, int z)
{
    //adds sides of block at positions x y z if needed. first removes all faces then adds if needed. do this for all 7 blocks.
    if(isInBounds(x, y, z))
    {
        unsigned int first_block = getBlock(x,y,z);
        glm::vec3 first_block_pos = glm::vec3(x,y,z);
        //loop over 6 sides
        std::vector<glm::vec3> positions = {{x+1, y,   z  },
                                            {x-1, y,   z  },
                                            {x  , y+1, z  },
                                            {x  , y-1, z  },
                                            {x  , y,   z+1},
                                            {x  , y,   z-1}};
        glm::ivec2 chunk_pos;
        glm::vec3 local_pos;
        std::tie(chunk_pos, local_pos) = findChunkPos(x,y,z);
        for(int face=0; face<6; face++)
        {
            //remove current face
            Chunk& chunk = chunk_map.at(chunk_pos);
            chunk.removeFromRenderMap(face, first_block_pos);
            chunk.re_init_vaos = true;

            glm::vec3 p = positions[face];
            if(isInBounds(p))
            {
                unsigned int block = getBlock(p);
                if(block == 0 && first_block != 0)  //TODO change to visible/nonvisible blocks?? (fence)
                {
                    //add face if checked block isnt air
                    chunk.addToRenderMap(first_block, face, first_block_pos);
                }
            }
            else if(p.y == settings.getChunkHeight() && first_block != 0)
            {
                chunk.addToRenderMap(first_block, face, first_block_pos);  //TODO change lighting
            }
        }
    }
}



std::vector<glm::vec2> find_grid_noise(float x, float y, float w, float h, float G, float p, unsigned int seed) //note: grid size G>1
{   
    std::vector<glm::vec2> points;
    float start_x = std::ceil(x/G)*G;
    float start_y = std::ceil(y/G)*G;
    for(float i=start_x; i<x+w; i+=G){
        for(float j=start_y; j<y+h; j+=G){
            glm::ivec2 point = glm::ivec2((int)i, (int)j);
            unsigned int prng = ivec2ToUIntPRNG(point)+seed;
            float f1 = p*(randFloatAccumulate(prng)*2-1);
            float f2 = p*(randFloatAccumulate(prng)*2-1);
            points.push_back(glm::vec2(i+f1, j+f2));
        }
    }

    return points;
}

unsigned int ChunkManager::determineBlock(int x, int y, int z, int water_level, float low_snow_level, float high_snow_level, 
    float* height_map, float* rng_map) //x y z local
{
    int ch_width = settings.getChunkWidth();
    int idx = z*ch_width+x;
    int height = height_map[idx];
    float prng = rng_map[idx];
    unsigned int blockID = blockIDMap["Air"];
    if(y<=water_level)
        blockID = blockIDMap["Water"];

    if(y<=height){
        if(height<water_level+2){
            if(height-3<y)
                blockID=blockIDMap["Sand"];
            else
                blockID=blockIDMap["Stone"];
        }
        else{
            if(height==y){
                float val = (y-low_snow_level)/(high_snow_level-low_snow_level);
                if(val>prng)
                    blockID=blockIDMap["Snowy Grass"];
                else
                    blockID=blockIDMap["Grass"];
            }
            else if(height-3<y)
                blockID=blockIDMap["Dirt"];
            else
                blockID=blockIDMap["Stone"];
        }
    }
    return blockID;
}

void ChunkManager::stageOneGen(glm::ivec2& pos)
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    int ch_depth = settings.getChunkDepth();
    shared_mutex.lock_shared();
    ChunkGenInfo& info = gen_info_map.at(pos);
    shared_mutex.unlock_shared();

    //hight map
    float* height_map = new float[ch_width*ch_depth];
    float* rng_map = new float[ch_width*ch_depth];
    for(int x=0; x<ch_width; x++)
    {
        for(int z=0; z<ch_depth; z++)
        {
            unsigned int count=0;
            int idx = z*ch_width+x;
            glm::ivec2 global_pos(x+pos.x*ch_width, z+pos.y*ch_depth);
            height_map[idx] = simplexOctaves2D(global_pos.x, global_pos.y, count, sizes, amplitudes);
            unsigned int prng = ivec2ToUIntPRNG(global_pos);
            rng_map[idx] = randFloatAccumulate(prng);
        }
    }

    //populate blocks
    int water_level = 40;
    float low_snow_level = 55;
    float high_snow_level=75;
    unsigned int* blocks = new unsigned int[ch_width*ch_height*ch_depth];
    for(int x=0; x<ch_width; x++){
        for(int z=0; z<ch_depth; z++){
            for(int y=0; y<ch_height; y++){
                unsigned int blockID = determineBlock(x, y, z, water_level, low_snow_level, high_snow_level, height_map, rng_map);
                blocks[x+ch_width*y+ch_width*ch_height*z] = blockID;
            }
        }
    }

    //trees
    float G=16;
    float p=5;
    info.tree_points = find_grid_noise(pos.x*ch_width, pos.y*ch_depth, ch_width, ch_depth, G, p, 0);

    //done
    info.height_map=height_map;
    info.rng_map=rng_map;
    info.blocks=blocks;
    info.stage = 1;
    busy.at(pos).store(false);
    work_queue_size.fetch_sub(1);
}


bool ChunkManager::isInChunk(glm::ivec2 ch_pos, int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();

    int chunk_x = std::floor(x / (float)ch_width);
    int chunk_z = std::floor(z / (float) ch_depth);
    glm::ivec2 pos = glm::ivec2(chunk_x, chunk_z);
    if (y>=0 && y<ch_height)
        if(ch_pos==pos)
            return true;
    return false;
}

void ChunkManager::setInfoGenBlock(int x, int y, int z, unsigned int blockID) //global
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    glm::ivec2 chunk_pos;
    glm::vec3 local;
    std::tie(chunk_pos, local) = findChunkPos(x,y,z);
    shared_mutex.lock_shared();
    ChunkGenInfo& info = gen_info_map.at(chunk_pos);
    shared_mutex.unlock_shared();
    int idx = local.x+ch_width*local.y+ch_width*ch_height*local.z;
    info.blocks[idx] = blockID;
}
unsigned int ChunkManager::getInfoGenBlock(int x, int y, int z)
{
    int ch_width = settings.getChunkWidth();
    int ch_height = settings.getChunkHeight();
    glm::ivec2 chunk_pos;
    glm::vec3 local;
    std::tie(chunk_pos, local) = findChunkPos(x,y,z);
    shared_mutex.lock_shared();
    ChunkGenInfo& info = gen_info_map.at(chunk_pos);
    shared_mutex.unlock_shared();
    int idx = local.x+ch_width*local.y+ch_width*ch_height*local.z;
    return info.blocks[idx];
}

void ChunkManager::placeTree(glm::ivec2& pos, int x, int z) //global x z
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    if((pos.x-1)*ch_width<=x && x<(pos.x+2)*ch_width){
        if((pos.y-1)*ch_depth<=z && z<(pos.y+2)*ch_depth){
            int water_level = 40;
            float low_snow_level = 55;
            float high_snow_level=75;
            glm::ivec2 chunk_pos; //of x z
            glm::vec3 local_pos;
            std::tie(chunk_pos, local_pos) = findChunkPos(x,0,z);
            shared_mutex.lock_shared();
            ChunkGenInfo& info = gen_info_map.at(chunk_pos);
            shared_mutex.unlock_shared();
            int idx = local_pos.z*ch_width+local_pos.x;
            int height = info.height_map[idx];
            unsigned int basePosBlock = determineBlock(local_pos.x, height, local_pos.z, water_level, low_snow_level, high_snow_level, 
                                                       info.height_map, info.rng_map);
            if(basePosBlock==blockIDMap["Grass"]){
                int r =3;
                int trunk_h=4;
                int bush_h=4;
                for(int i=-r; i<=r; i++){
                    for(int j=-r; j<=r; j++){
                        for(int k=0; k<bush_h; k++){
                            if(isInChunk(pos, x+i, height+trunk_h+1+k, z+j)){
                                setInfoGenBlock(x+i, height+trunk_h+1+k, z+j, blockIDMap["Oak Leaves"]);
                            }
                        }
                    }
                }
                for(int i=0; i<trunk_h+bush_h/2.0; i++){
                    if(isInChunk(pos, x, height+i+1, z)){
                        setInfoGenBlock(x, height+i+1, z, blockIDMap["Oak Log"]);
                    }
                }
            }           
        }
    }
}

void ChunkManager::stageTwoDecoration(glm::ivec2& pos)
{

    for(int ch_x=-1; ch_x<=1; ch_x++){
        for(int ch_y=-1; ch_y<=1; ch_y++){
            glm::ivec2 iter_pos(pos.x+ch_x, pos.y+ch_y);
            shared_mutex.lock_shared();
            ChunkGenInfo& iter_info = gen_info_map.at(iter_pos);
            shared_mutex.unlock_shared();
            for(glm::vec2& tree_pos : iter_info.tree_points){
                placeTree(pos, tree_pos.x, tree_pos.y);
            }
        }
    }
    //done
    shared_mutex.lock_shared();
    ChunkGenInfo& info = gen_info_map.at(pos);
    shared_mutex.unlock_shared();
    info.stage = 2;
    busy[pos].store(false);
    work_queue_size.fetch_sub(1);
}


void ChunkManager::visibilityInnerAtPos(ChunkGenInfo& info, glm::ivec2& pos, int face, int x, int y, int z, unsigned int blockID) //local coordinates
{   
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();
    unsigned int& air = blockIDMap["Air"];
    if(blockID!=air){
        std::vector<glm::vec3> positions = {{x+1, y,   z  },
                                            {x-1, y,   z  },
                                            {x  , y+1, z  },
                                            {x  , y-1, z  },
                                            {x  , y,   z+1},
                                            {x  , y,   z-1}};
        glm::vec3 p(positions[face]);
        glm::vec3 global_pos(pos.x*ch_width + x, y, pos.y*ch_depth + z);
        
        if(isInChunk(pos, pos.x*ch_width + p.x, p.y, pos.y*ch_depth + p.z))
        {   
            int idx = p.x+ch_width*p.y+ch_width*ch_height*p.z;
            unsigned int& block = info.blocks[idx];
            if(block == air) //TODO TODO change to visible/transparant block
                addToRenderMap(info, blockID, face, global_pos);
        }
        else if(face == BlockModel::TOP && y == settings.getChunkHeight()-1) //2 = BlockModel::TOP
        {
            addToRenderMap(info, blockID, 2, global_pos);
        }
    }
}

void ChunkManager::addToRenderMap(ChunkGenInfo& info, int blockID, int face, glm::vec3 pos)
{
    info.render_faces_map[face][pos] = blockID;
}


void ChunkManager::visibilityInner(ChunkGenInfo& info, glm::ivec2& pos)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();
    for(int x=0; x<ch_width; x++){
        for(int z=0; z<ch_depth; z++){   
            for(int y=0; y<ch_height; y++){
                int idx = x+ch_width*y+ch_width*ch_height*z;
                unsigned int& block = info.blocks[idx];
                for(int face=0; face<6; face++){
                    visibilityInnerAtPos(info, pos, face, x, y, z, block);
                }
            }
        }
    }
}

void ChunkManager::visibilityOuter(ChunkGenInfo& info, glm::ivec2& pos, ChunkGenInfo& info2, int face)
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();
    glm::vec3 start;
    glm::vec3 iter;
    glm::vec3 start2;
    int W;
    if(face==BlockModel::EAST){
        W=ch_depth;
        start = glm::vec3(ch_width-1, 0, ch_depth-1);
        iter = glm::vec3(0,0,-1);
        start2 = glm::vec3(0,0,ch_depth-1);
    }else if(face==BlockModel::WEST){
        W=ch_depth;
        start = glm::vec3(0,0,0);
        iter = glm::vec3(0,0,1);
        start2 = glm::vec3(ch_width-1, 0, 0);
    }else if(face==BlockModel::NORTH){
        W=ch_width;
        start = glm::vec3(ch_width-1, 0, ch_depth-1);
        iter = glm::vec3(-1,0,0);
        start2 = glm::vec3(ch_width-1, 0, 0);
    }else{ //south
        W=ch_width;
        start = glm::vec3(0,0,0);
        iter = glm::vec3(1,0,0);
        start2 = glm::vec3(0,0,ch_depth-1);
    }
    for(int y=0; y<ch_height; y++){
        glm::vec3 p(start.x, y, start.z);
        glm::vec3 p2(start2.x, y, start2.z);
        for(int i=0; i<W; i++){
            int idx = p.x+ch_width*p.y+ch_width*ch_height*p.z;
            int idx2 = p2.x+ch_width*p2.y+ch_width*ch_height*p2.z;
            if(info.blocks[idx]!=blockIDMap["Air"]){
                if(info2.blocks[idx2]==blockIDMap["Air"]){
                    addToRenderMap(info, info.blocks[idx], face, {pos.x*ch_width + p.x, p.y, pos.y*ch_depth + p.z});
                }
            }
            p+=iter;
            p2+=iter;
        }
    }
}

void ChunkManager::stageThreeChunkCreation(glm::ivec2& pos) //dont actually need all 8 adjacent, only 4
{
    int ch_width = settings.getChunkWidth();
    int ch_depth = settings.getChunkDepth();
    int ch_height = settings.getChunkHeight();
    shared_mutex.lock_shared();
    ChunkGenInfo& info = gen_info_map.at(pos);
    ChunkGenInfo& info_e = gen_info_map.at({pos.x+1, pos.y});
    ChunkGenInfo& info_w = gen_info_map.at({pos.x-1, pos.y});
    ChunkGenInfo& info_n = gen_info_map.at({pos.x, pos.y+1});
    ChunkGenInfo& info_s = gen_info_map.at({pos.x, pos.y-1});
    shared_mutex.unlock_shared();

    //inner visibility check
    visibilityInner(info, pos);

    //4 outer visibility checks
    visibilityOuter(info, pos, info_e, BlockModel::EAST);
    visibilityOuter(info, pos, info_w, BlockModel::WEST);
    visibilityOuter(info, pos, info_n, BlockModel::NORTH);
    visibilityOuter(info, pos, info_s, BlockModel::SOUTH);
    //TODO

    //done
    info.stage = 3;
    delete info.height_map;
    delete info.rng_map;
    busy[pos].store(false);
    work_queue_size.fetch_sub(1);
}

void ChunkManager::gen_nearby_chunks(glm::vec3& center_pos)
{
    int x_ch = std::floor(center_pos.x / (float)settings.getChunkWidth());
    int z_ch = std::floor(center_pos.z / (float)settings.getChunkDepth());
    for(glm::ivec2& cir_pos: circle_chunk_positions){
        glm::ivec2 pos(cir_pos.x + x_ch, cir_pos.y + z_ch);
        if(work_queue_size.load()<n_threads*2){
            if(busy.count(pos)==0){//if never visited
                busy[pos].store(true);
                work_queue_size.fetch_add(1);
                shared_mutex.lock();
                gen_info_map[pos].stage=0;
                shared_mutex.unlock();
                pool.ioService.post(boost::bind(&ChunkManager::stageOneGen, this, pos));
            }
            else if(busy.at(pos).load()==false){ //if not busy
                bool ready=true;
                ChunkGenInfo& info = gen_info_map.at(pos);
                int stage = info.stage;
                if(stage<4){
                    for(int ch_x=-1; ch_x<=1; ch_x++){
                        for(int ch_y=-1; ch_y<=1; ch_y++){
                            glm::ivec2 iter_pos(pos.x+ch_x, pos.y+ch_y);
                            if(gen_info_map.count(iter_pos)>0){
                                if(gen_info_map.at(iter_pos).stage<stage){
                                    ready=false;
                                }
                            } else{
                                ready=false;
                            }
                        }
                    }
                    if(ready){
                        if(stage==1){
                            work_queue_size.fetch_add(1);
                            busy.at(pos).store(true);
                            pool.ioService.post(boost::bind(&ChunkManager::stageTwoDecoration, this, pos));
                        }else if(stage==2){
                            work_queue_size.fetch_add(1);
                            busy.at(pos).store(true);
                            pool.ioService.post(boost::bind(&ChunkManager::stageThreeChunkCreation, this, pos));
                        }else if(stage==3 && !info.added_to_queue){
                            finished_positions.push(pos);
                            info.added_to_queue=true;
                        }
                    }
                }
            }
        }
    }
}


void ChunkManager::add_new_chunks()
{
    int max_per_frame = 3;
    int size = finished_positions.size();
    for(int i=0; i<max_per_frame && i<size; i++){
        //construct in-place
        glm::ivec2& pos = finished_positions.front();
        ChunkGenInfo& info = gen_info_map.at(pos);
        chunk_map.emplace(std::piecewise_construct,
                          std::forward_as_tuple(pos),
                          std::forward_as_tuple(settings, pos, info.blocks, info.num_render_faces, info.render_faces_map));
        finished_positions.pop();
    }
}


void ChunkManager::remove_far_chunks(glm::vec3& center_pos)
{
    int max_per_frame = 100;
    float& x = center_pos.x;
    float& z = center_pos.z;
    int x_ch = std::floor(x / (float)settings.getChunkWidth());
    int z_ch = std::floor(z / (float)settings.getChunkDepth());
    // given R, make a square with sidelength 2R. then filter out dist. then sort dist
    int R = settings.getRenderDistance();
    glm::ivec2 player_pos_ch = glm::ivec2(x_ch, z_ch);
    int count=0;
    std::vector<glm::ivec2> remove_chunk_positions;
    std::vector<glm::ivec2> remove_only_info_positions;
    for(auto& pair : gen_info_map)
    {
        if(count<max_per_frame){
            glm::ivec2 pos = pair.first;
            if(glm::length(glm::vec2(pos-player_pos_ch))>=R)
            {   
                bool ready=true;
                for(int ch_x=-1; ch_x<=1; ch_x++){
                    for(int ch_y=-1; ch_y<=1; ch_y++){
                        glm::ivec2 iter_pos(pos.x+ch_x, pos.y+ch_y);
                        if(busy.count(iter_pos)>0){
                            if(busy.at(iter_pos).load()){
                                ready=false;
                            }
                        }
                    }
                }
                if(ready){
                    int stage = gen_info_map.at(pos).stage;
                    if(stage==3 && chunk_map.count(pos)>0 ){
                        remove_chunk_positions.push_back(pos);
                    }
                    else{
                        remove_only_info_positions.push_back(pos);
                    }
                    count++;
                }
            }
        } else{
            break;
        }
    }

    for(auto& pos : remove_chunk_positions){
        ChunkGenInfo& info = gen_info_map.at(pos);
        chunk_map.erase(pos);
        gen_info_map.erase(pos);
        busy.erase(pos);
    }
    for(auto& pos : remove_only_info_positions){
        ChunkGenInfo& info = gen_info_map.at(pos);
        if(info.stage!=3){
            delete info.rng_map;
            delete info.height_map;
        }
        delete info.blocks;
        gen_info_map.erase(pos);
        busy.erase(pos);
    }
}


void ChunkManager::end()
{
    pool.stop();
    for(auto& pair : gen_info_map){
        glm::ivec2 pos = pair.first;
        ChunkGenInfo& info = pair.second;
        if(chunk_map.count(pos)==0){
            delete info.blocks;
            if(info.stage!=3){
                delete info.rng_map;
                delete info.height_map;
            }
        }
    }
}