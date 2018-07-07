#include "src/engine/Landscape.h"

Landscape::Landscape()
{
}

Mesh Landscape::CreateBasicShape()
{
    // generate the sphere data
    GLfloat x,y,z,a,b,da,db,r = 3.5;
    const int na=36;
    const int nb=18;
    int ia,ib,ix,iy;
    da = 2.0*M_PI/GLfloat(na);
    db = M_PI/GLfloat(nb-1);
    GLuint sphere_ix [na*(nb-1)*6];

    // spherical angles a,b covering whole sphere surface
    for (ix=0,b=-0.5*M_PI,ib=0;ib<nb;ib++,b+=db)
    {
        for (a=0.0,ia=0;ia<na;ia++,a+=da,ix+=3)
        {   
            Vertex vertex;

            // unit sphere
            x=cos(b)*cos(a);
            y=cos(b)*sin(a);
            z=sin(b);

            glm::vec3 pos;
            pos.x = x*r;
            pos.y = y*r;
            pos.z = z*r;
            vertex.position = pos;

            glm::vec3 normal;
            normal.x = x;
            normal.y = y;
            normal.z = z;
            vertex.normal = normal;

            vertex.tex_coords = glm::vec2(0.0f, 0.0f);
            vertex.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            vertex.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);

            vertices.push_back(vertex);
        }
    }

    // [Generate GL_TRIANGLE indices]
    for (ix=0,iy=0,ib=1;ib<nb;ib++)
    {
        for (ia=1;ia<na;ia++,iy++)
        {
            //// first half of QUAD
            //indices.push_back(iy);
            //ix++;
            //indices.push_back(iy + 1);
            //ix++;
            //indices.push_back(iy + na);
            //ix++;
//
            //indices.push_back(iy + na);
            //ix++;
            //indices.push_back(iy + 1);
            //ix++;
            //indices.push_back(iy + na + 1);
            //ix++;

            sphere_ix[ix]=iy;      
            ix++;
            sphere_ix[ix]=iy+1;
            ix++;
            sphere_ix[ix]=iy+na;   
            ix++;
            // second half of QUAD
            sphere_ix[ix]=iy+na;   
            ix++;
            sphere_ix[ix]=iy+1;    
            ix++;
            sphere_ix[ix]=iy+na+1; 
            ix++;
        }

        //indices.push_back(iy);
        //ix++;
        //indices.push_back(iy + 1 - na);
        //ix++;
        //indices.push_back(iy + na);
        //ix++;
//
        //indices.push_back(iy + na);
        //ix++;
        //indices.push_back(iy - na + 1);
        //ix++;
        //indices.push_back(iy + 1);
        //ix++;
        //iy++;

        // first half of QUAD
        sphere_ix[ix]=iy;       
        ix++;
        sphere_ix[ix]=iy+1-na;  
        ix++;
        sphere_ix[ix]=iy+na;    
        ix++;

        // second half of QUAD
        sphere_ix[ix]=iy+na;    
        ix++;
        sphere_ix[ix]=iy-na+1;  
        ix++;
        sphere_ix[ix]=iy+1;     
        ix++;
        iy++;
    }

    std::vector<unsigned int> sadasda(std::begin(sphere_ix), std::end(sphere_ix));

    return Mesh(vertices, sadasda, textures);
}