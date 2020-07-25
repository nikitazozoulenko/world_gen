#include <proceduralGeneration.h>


unsigned int wang_hash(unsigned int seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

unsigned int randUIntAccumulate(unsigned int& rand_state)
{   // wang_hash uint32_t
    // Uniform [0, MAX_UINT32_T]
    rand_state = wang_hash(rand_state);
    return rand_state;
}


unsigned int ivec2ToUIntPRNG(glm::ivec2 v)
{
    return wang_hash(wang_hash(v.x)+v.y);
}


unsigned int ivec3ToUIntPRNG(glm::ivec3 v)
{
    return wang_hash(wang_hash(wang_hash(v.x)+v.y)+v.z);
}


float randFloatAccumulate(unsigned int& rand_state)
{   // wang_hash uint32_t
    // Uniform in range [0.0, 1.0]
    double n = randUIntAccumulate(rand_state);
    unsigned int MAX = -1;
    return float(n/MAX);
}


float simplexNoise2D(float x, float y, float& amplitude, unsigned int& count) //"world" x and y, can be scaled
{
    float noise = 0.0;

    //scewed coordinates 
    float NNN = 2.0f;
    float F = (sqrt(NNN+1)-1)/NNN;
    float G = (1-1/sqrt(NNN+1))/NNN;
    float x_scew = x + (x+y)*F;
    float y_scew = y + (x+y)*F;

    //which hypercube coordinates 
    float x_b = floor(x_scew);
    float y_b = floor(y_scew);

    //internal coordinates
    float x_i = x_scew-x_b;
    float y_i = y_scew-y_b;

    //order for edge traversal
    const int N_CORNERS = 3;
    glm::ivec2 corners[N_CORNERS];
    corners[0] = glm::ivec2(x_b, y_b);
    corners[1];
    corners[2] = corners[0] + glm::ivec2(1,1);
    if(x_i > y_i)
        corners[1] = corners[0] + glm::ivec2(1, 0);
    else
        corners[1] = corners[0] + glm::ivec2(0, 1);

    //traverse every edge
    for(glm::ivec2& vert : corners)
    {
        float factor = (vert.x+vert.y)*G;
        float x_n = x-(vert.x - factor);
        float y_n = y-(vert.y - factor);

        float M = glm::max(0.0f, 0.5f - x_n*x_n - y_n*y_n);
        unsigned int PRNG = ivec2ToUIntPRNG(vert)+count;
        float f0 = randFloatAccumulate(PRNG)*2-1;
        float f1 = randFloatAccumulate(PRNG)*2-1;
        noise += M*M*M*M * glm::dot(glm::vec2(f0, f1), glm::vec2(x_n, y_n));
    }

    count+=1;
    //scale to [0, amplitude]
    return (78.0 * noise +1.0)/2.0 * amplitude;
}


float simplexNoise3D(float x, float y, float z, float& amplitude, unsigned int& count) //"world" x, y and z, can be scaled
{
    float noise = 0.0;

    //scewed coordinates 
    float NNN = 3.0;
    float F = (sqrt(NNN+1)-1)/NNN;
    float G = (1-1/sqrt(NNN+1))/NNN;
    float x_scew = x + (x+y+z)*F;
    float y_scew = y + (x+y+z)*F;
    float z_scew = z + (x+y+z)*F;

    //which hypercube coordinates 
    float x_b = floor(x_scew);
    float y_b = floor(y_scew);
    float z_b = floor(z_scew);

    //internal coordinates
    float x_i = x_scew-x_b;
    float y_i = y_scew-y_b;
    float z_i = z_scew-z_b;

    //order for edge traversal
    const int N_CORNERS = 4;
    glm::ivec3 corners[N_CORNERS];
    corners[0] = glm::ivec3(x_b, y_b, z_b);
    corners[1];
    corners[2];
    corners[3] = corners[0] + glm::ivec3(1, 1, 1);
    if(x_i >= y_i && x_i >= z_i)
    {
        corners[1] = corners[0] + glm::ivec3(1, 0, 0);
        if(y_i >= z_i)
            corners[2] = corners[0] + glm::ivec3(1, 1, 0);
        else
            corners[2] = corners[0] + glm::ivec3(1, 0, 1);
    }
    else if(y_i >= x_i && y_i >= z_i)
    {
        corners[1] = corners[0] + glm::ivec3(0, 1, 0);
        if(x_i >= z_i)
            corners[2] = corners[0] + glm::ivec3(1, 1, 0);
        else
            corners[2] = corners[0] + glm::ivec3(0, 1, 1);
    }
    else // (z_i > x_i && z_i > y_i)
    {
        corners[1] = corners[0] + glm::ivec3(0, 0, 1);
        if(x_i >= y_i)
            corners[2] = corners[0] + glm::ivec3(1, 0, 1);
        else
            corners[2] = corners[0] + glm::ivec3(0, 1, 1);
    }

    //traverse every edge
    for(glm::ivec3& vert : corners)
    {
        float factor = (vert.x+vert.y+vert.z)*G;
        float x_n = x-(vert.x - factor);
        float y_n = y-(vert.y - factor);
        float z_n = z-(vert.z - factor);

        float M = glm::max(0.0f, 0.5f - x_n*x_n - y_n*y_n - z_n*z_n);
        unsigned int PRNG = ivec3ToUIntPRNG(vert)+count;
        float f0 = randFloatAccumulate(PRNG)*2-1;
        float f1 = randFloatAccumulate(PRNG)*2-1;
        float f2 = randFloatAccumulate(PRNG)*2-1;
        noise += M*M*M*M * glm::dot(glm::vec3(f0, f1, f2), glm::vec3(x_n, y_n, z_n));
    }

    count +=1;
    //scale to [0, amplitude]
    return (74.0*noise +1.0)/2.0 * amplitude;
}


float simplexOctaves2D(float world_x, float world_y, unsigned int& count, std::vector<float>& sizes, std::vector<float>& amplitudes)
{
    float noise = 0.0;
    for (int i=0; i<amplitudes.size(); i++)
    {
        float size = sizes[i];
        float amplitude = amplitudes[i];
        noise += simplexNoise2D(world_x/size, world_y/size, amplitude, count);
    }
    return noise;
}


// // Modulo 289 without a division (only multiplications)
// vec3 mod289(vec3 x) {
//   return x - floor(x * (1.0 / 289.0)) * 289.0;
// }

// vec2 mod289(vec2 x) {
//   return x - floor(x * (1.0 / 289.0)) * 289.0;
// }

// // Modulo 7 without a division
// vec3 mod7(vec3 x) {
//   return x - floor(x * (1.0 / 7.0)) * 7.0;
// }

// // Permutation polynomial: (34x^2 + x) mod 289
// vec3 permute(vec3 x) {
//   return mod289((34.0 * x + 1.0) * x);
// }

// // Cellular noise, returning F1 and F2 in a vec2.
// // Standard 3x3 search window for good F1 and F2 values
// vec2 cellular(vec2 P, float amplitude) {
// #define K 0.142857142857 // 1/7
// #define Ko 0.428571428571 // 3/7
// #define jitter 1.0 // Less gives more regular pattern
// 	vec2 Pi = mod289(floor(P));
//  	vec2 Pf = fract(P);
// 	vec3 oi = vec3(-1.0, 0.0, 1.0);
// 	vec3 of = vec3(-0.5, 0.5, 1.5);
// 	vec3 px = permute(Pi.x + oi);
// 	vec3 p = permute(px.x + Pi.y + oi); // p11, p12, p13
// 	vec3 ox = fract(p*K) - Ko;
// 	vec3 oy = mod7(floor(p*K))*K - Ko;
// 	vec3 dx = Pf.x + 0.5 + jitter*ox;
// 	vec3 dy = Pf.y - of + jitter*oy;
// 	vec3 d1 = dx * dx + dy * dy; // d11, d12 and d13, squared
// 	p = permute(px.y + Pi.y + oi); // p21, p22, p23
// 	ox = fract(p*K) - Ko;
// 	oy = mod7(floor(p*K))*K - Ko;
// 	dx = Pf.x - 0.5 + jitter*ox;
// 	dy = Pf.y - of + jitter*oy;
// 	vec3 d2 = dx * dx + dy * dy; // d21, d22 and d23, squared
// 	p = permute(px.z + Pi.y + oi); // p31, p32, p33
// 	ox = fract(p*K) - Ko;
// 	oy = mod7(floor(p*K))*K - Ko;
// 	dx = Pf.x - 1.5 + jitter*ox;
// 	dy = Pf.y - of + jitter*oy;
// 	vec3 d3 = dx * dx + dy * dy; // d31, d32 and d33, squared
// 	// Sort out the two smallest distances (F1, F2)
// 	vec3 d1a = min(d1, d2);
// 	d2 = max(d1, d2); // Swap to keep candidates for F2
// 	d2 = min(d2, d3); // neither F1 nor F2 are now in d3
// 	d1 = min(d1a, d2); // F1 is now in d1
// 	d2 = max(d1a, d2); // Swap to keep candidates for F2
// 	d1.xy = (d1.x < d1.y) ? d1.xy : d1.yx; // Swap if smaller
// 	d1.xz = (d1.x < d1.z) ? d1.xz : d1.zx; // F1 is in d1.x
// 	d1.yz = min(d1.yz, d2.yz); // F2 is now not in d2.yz
// 	d1.y = min(d1.y, d1.z); // nor in  d1.z
// 	d1.y = min(d1.y, d2.x); // F2 is in d1.y, we're done.
// 	return sqrt(d1.xy)*amplitude;
// }

