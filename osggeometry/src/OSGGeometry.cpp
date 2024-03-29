/* \brief Main file of an example that demonstrates how to draw thick and smooth curves in 3D.
 * This file is a part of shader-3dcurve example (https://github.com/vicrucann/shader-3dcurve).
 *
 * \author Victoria Rudakova
 * \date January 2017
 * \copyright MIT license
*/

#ifdef _WIN32
#include <Windows.h>
#endif // _WIN32

#include <osg/Geode>
#include <osg/Geometry>
#include <osg/Material>
#include <osg/Vec3>
#include <osg/MatrixTransform>
#include <osg/Texture2D>
#include <osg/PolygonStipple>
#include <osg/TemplatePrimitiveFunctor>
#include <osg/TemplatePrimitiveIndexFunctor>
#include <osg/io_utils>

#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#include <osgGA/TrackballManipulator>

#include <osgViewer/Viewer>

#include <osg/Math>

#include <iostream>

struct NormalPrint
{
    void operator() (const osg::Vec3& v1, bool) const
    {
        std::cout << "\rpoint("<<v1<<")"<<std::endl;
    }

    void operator() (const osg::Vec3& v1,const osg::Vec3& v2, bool) const
    {
        std::cout << "\tline("<<v1<<") ("<<v2<<")"<<std::endl;
    }

    void operator() (const osg::Vec3& v1,const osg::Vec3& v2,const osg::Vec3& v3, bool) const
    {
        osg::Vec3 normal = (v2-v1)^(v3-v2);
        normal.normalize();
        std::cout << "\ttriangle("<<v1<<") ("<<v2<<") ("<<v3<<") "<<") normal ("<<normal<<")"<<std::endl;
    }

    void operator() (const osg::Vec3& v1,const osg::Vec3& v2,const osg::Vec3& v3,const osg::Vec3& v4, bool) const
    {
        osg::Vec3 normal = (v2-v1)^(v3-v2);
        normal.normalize();
        std::cout << "\tquad("<<v1<<") ("<<v2<<") ("<<v3<<") ("<<v4<<") "<<")"<<std::endl;
    }
};

// decompose Drawable primitives into triangles, print out these triangles and computed normals.
void printPrimitives(const std::string& name, osg::Drawable& drawable)
{
    std::cout<<name<<std::endl;

    osg::TemplatePrimitiveFunctor<NormalPrint> tf;
    drawable.accept(tf);

    std::cout<<std::endl;
}

struct PrimitiveIndexPrint
{
    void operator() (unsigned int p1) const
    {
        std::cout << "\tpoint("<<p1<<")"<<std::endl;
    }
    void operator() (unsigned int p1, unsigned int p2) const
    {
        std::cout << "\tline("<<p1<<", "<<p2<<")"<<std::endl;
    }
    void operator() (unsigned int p1, unsigned int p2, unsigned int p3) const
    {
        std::cout << "\ttriangle("<<p1<<", "<<p2<<", "<<p3<<")"<<std::endl;
    }
    void operator() (unsigned int p1, unsigned int p2, unsigned int p3, unsigned int p4) const
    {
        std::cout << "\tquad("<<p1<<", "<<p2<<", "<<p3<<", "<<p4<<")"<<std::endl;
    }
};

// decompose Drawable primitives into triangles, print out these triangles and computed normals.
void printPrimitiveIndices(const std::string& name, osg::Drawable& drawable)
{
    std::cout<<name<<std::endl;

    osg::TemplatePrimitiveIndexFunctor<PrimitiveIndexPrint> pf;
    drawable.accept(pf);

    std::cout<<std::endl;
}

/// Create a scene with examples of the different types of OpenGL primitives.
/// The primitives are the same as shown in the OpenGL diagram loaded in
/// createBackground() (Images/primitives.gif).
osg::Node* createScene()
{
    // create the Geode (Geometry Node) to contain all our osg::Geometry objects.
    osg::Geode* geode = new osg::Geode();

    // following are separate blocks for creating POINTS, LINES, LINE_STRIP, LINE_LOOP, POLYGON, QUADS,
    // QUAD_STRIP, TRIANGLES, TRIANGLE_STRIP and TRIANGLE_FAN primitives. An image of these primitives
    // is provided in the distribution: OpenSceneGraph-Data/Images/primitives.gif.


    // create POINTS
    {
        // create Geometry object to store all the vertices and points primitive.
        osg::Geometry* pointsGeom = new osg::Geometry();

        // create a Vec3Array and add to it all my coordinates.
        // Like all the *Array variants (see include/osg/Array) , Vec3Array is derived from both osg::Array
        // and std::vector<>.  osg::Array's are reference counted and hence sharable,
        // which std::vector<> provides all the convenience, flexibility and robustness
        // of the most popular of all STL containers.
        osg::Vec3Array* vertices = new osg::Vec3Array;
        vertices->push_back(osg::Vec3(-1.02168, -2.15188e-09, 0.885735));
        vertices->push_back(osg::Vec3(-0.976368, -2.15188e-09, 0.832179));
        vertices->push_back(osg::Vec3(-0.873376, 9.18133e-09, 0.832179));
        vertices->push_back(osg::Vec3(-0.836299, -2.15188e-09, 0.885735));
        vertices->push_back(osg::Vec3(-0.790982, 9.18133e-09, 0.959889));

        // pass the created vertex array to the points geometry object.
        pointsGeom->setVertexArray(vertices);



        // create the color of the geometry, one single for the whole geometry.
        // for consistency of design even one single color must added as an element
        // in a color array.
        osg::Vec4Array* colors = new osg::Vec4Array;
        // add a white color, colors take the form r,g,b,a with 0.0 off, 1.0 full on.
        colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));

        // pass the color array to points geometry, note the binding to tell the geometry
        // that only use one color for the whole object.
        pointsGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


        // Set the normal in the same way as the color.
        // (0,-1,0) points toward the viewer, in the default coordinate
        // setup.  Even for POINTS, the normal specified here
        // is used to determine how the geometry appears under different
        // lighting conditions.
        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
        pointsGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);


        // create and add a DrawArray Primitive (see include/osg/Primitive).  The first
        // parameter passed to the DrawArrays constructor is the Primitive::Mode which
        // in this case is POINTS (which has the same value GL_POINTS), the second
        // parameter is the index position into the vertex array of the first point
        // to draw, and the third parameter is the number of points to draw.
        pointsGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POINTS,0,vertices->size()));

        printPrimitiveIndices("POINTS indices", *pointsGeom);

        // add the points geometry to the geode.
        geode->addDrawable(pointsGeom);
    }

    // create LINES
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* linesGeom = new osg::Geometry();

        // this time we'll preallocate the vertex array to the size we
        // need and then simple set them as array elements, 8 points
        // makes 4 line segments.
        osg::Vec3Array* vertices = new osg::Vec3Array(8);
        (*vertices)[0].set(-1.13704, -2.15188e-09, 0.40373);
        (*vertices)[1].set(-0.856897, -2.15188e-09, 0.531441);
        (*vertices)[2].set(-0.889855, -2.15188e-09, 0.444927);
        (*vertices)[3].set(-0.568518, -2.15188e-09, 0.40373);
        (*vertices)[4].set(-1.00933, -2.15188e-09, 0.370773);
        (*vertices)[5].set(-0.716827, -2.15188e-09, 0.292498);
        (*vertices)[6].set(-1.07936, 9.18133e-09, 0.317217);
        (*vertices)[7].set(-0.700348, 9.18133e-09, 0.362533);


        // pass the created vertex array to the points geometry object.
        linesGeom->setVertexArray(vertices);

        // set the colors as before, plus using the above
        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        linesGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


        // Set the normal in the same way as the color.
        // As above for POINTS, this normal is used for the lighting
        // calculations of the LINES.
        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
        linesGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINES,0,8));

        printPrimitiveIndices("LINES indices", *linesGeom);

        // add the points geometry to the geode.
        geode->addDrawable(linesGeom);
    }

    // create LINE_STRIP
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* linesGeom = new osg::Geometry();

        // this time we'll preallocate the vertex array to the size
        // and then use an iterator to fill in the values, a bit perverse
        // but does demonstrate that we have just a standard std::vector underneath.
        osg::Vec3Array* vertices = new osg::Vec3Array(5);
        osg::Vec3Array::iterator vitr = vertices->begin();
        (vitr++)->set(-0.0741545, -2.15188e-09, 0.416089);
        (vitr++)->set(0.234823, -2.15188e-09, 0.259541);
        (vitr++)->set(0.164788, -2.15188e-09, 0.366653);
        (vitr++)->set(-0.0288379, -2.15188e-09, 0.333695);
        (vitr++)->set(-0.0453167, -2.15188e-09, 0.280139);

        // pass the created vertex array to the points geometry object.
        linesGeom->setVertexArray(vertices);

        // set the colors as before, plus using the above
        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        linesGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


        // Set the normal in the same way as the color (see note at POINTS, above).
        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
        linesGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_STRIP,0,5));

        printPrimitiveIndices("LINE_STRIP indices", *linesGeom);

        // add the points geometry to the geode.
        geode->addDrawable(linesGeom);
    }

    // create LINE_LOOP
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* linesGeom = new osg::Geometry();

        // this time we'll a C arrays to initialize the vertices.

        osg::Vec3 myCoords[] =
        {
            osg::Vec3(0.741546, -2.15188e-09, 0.453167),
            osg::Vec3(0.840418, -2.15188e-09, 0.304858),
            osg::Vec3(1.12468, -2.15188e-09, 0.300738),
            osg::Vec3(1.03816, 9.18133e-09, 0.453167),
            osg::Vec3(0.968129, -2.15188e-09, 0.337815),
            osg::Vec3(0.869256, -2.15188e-09, 0.531441)
        };

        int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

        osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

        // pass the created vertex array to the points geometry object.
        linesGeom->setVertexArray(vertices);

        // set the colors as before, plus using the above
        osg::Vec4Array* colors = new osg::Vec4Array;
        colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));
        linesGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


        // Set the normal in the same way as the color (see note at POINTS, above).
        osg::Vec3Array* normals = new osg::Vec3Array;
        normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
        linesGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        linesGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::LINE_LOOP,0,numCoords));

        printPrimitiveIndices("LINE_LOOP indices", *linesGeom);

        // add the points geometry to the geode.
        geode->addDrawable(linesGeom);
    }




    // Now we'll stop creating separate normal and color arrays.
    // Since we are using the same values all the time, we'll just
    // share the same ColorArray and NormalArrays.

    // Set the colors as before, use a ref_ptr rather than just
    // standard C pointer, as that in the case of it not being
    // assigned it will still be cleaned up automatically.
    osg::ref_ptr<osg::Vec4Array> shared_colors = new osg::Vec4Array;
    shared_colors->push_back(osg::Vec4(1.0f,1.0f,0.0f,1.0f));

    // Same trick for shared normal.
    osg::ref_ptr<osg::Vec3Array> shared_normals = new osg::Vec3Array;
    shared_normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));

    // Note on vertex ordering.
    // According to the OpenGL diagram vertices should be specified in a clockwise direction.
    // In reality you need to specify coords for polygons in a anticlockwise direction
    // for their front face to be pointing towards you; get this wrong and you could
    // find back face culling removing the wrong faces of your models.  The OpenGL diagram
    // is just plain wrong, but it's a nice diagram so we'll keep it for now!

    // create POLYGON
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* polyGeom = new osg::Geometry();

        // this time we'll use C arrays to initialize the vertices.
        // note, anticlockwise ordering.
        // note II, OpenGL polygons must be convex, planar polygons, otherwise
        // undefined results will occur.  If you have concave polygons or ones
        // that cross over themselves then use the osgUtil::Tessellator to fix
        // the polygons into a set of valid polygons.
        osg::Vec3 myCoords[] =
        {
            osg::Vec3(-1.0464, 0.0f, -0.193626),
            osg::Vec3(-1.0258, 0.0f, -0.26778),
            osg::Vec3(-0.807461, 0.0f, -0.181267),
            osg::Vec3(-0.766264, 0.0f, -0.0576758),
            osg::Vec3(-0.980488, 0.0f, -0.094753)
        };

        int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

        osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

        // pass the created vertex array to the points geometry object.
        polyGeom->setVertexArray(vertices);

        // use the shared color array.
        polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);


        // use the shared normal array.
        polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::POLYGON,0,numCoords));

        printPrimitiveIndices("POLYGON indices", *polyGeom);
        printPrimitives("POLYGON vertices", *polyGeom);

        // add the points geometry to the geode.
        geode->addDrawable(polyGeom);
    }


    // create QUADS
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* polyGeom = new osg::Geometry();

        // note, anticlockwise ordering.
        osg::Vec3 myCoords[] =
        {
            osg::Vec3(0.0247182, 0.0f, -0.156548),
            osg::Vec3(0.0247182, 0.0f, -0.00823939),
            osg::Vec3(-0.160668, 0.0f, -0.0453167),
            osg::Vec3(-0.222464, 0.0f, -0.13183),

            osg::Vec3(0.238942, 0.0f, -0.251302),
            osg::Vec3(0.333696, 0.0f, 0.0329576),
            osg::Vec3(0.164788, 0.0f, -0.0453167),
            osg::Vec3(0.13595,  0.0f, -0.255421)
        };

        int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

        osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

        // pass the created vertex array to the points geometry object.
        polyGeom->setVertexArray(vertices);

        // use the shared color array.
        polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);


        // use the shared normal array.
        polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUADS,0,numCoords));


        printPrimitiveIndices("QUADS indices", *polyGeom);
        printPrimitives("QUADS vertices", *polyGeom);

        // add the points geometry to the geode.
        geode->addDrawable(polyGeom);
    }

    // create QUAD_STRIP
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* polyGeom = new osg::Geometry();

        // note, first coord at top, second at bottom, reverse to that buggy OpenGL image..
        osg::Vec3 myCoords[] =
        {
            osg::Vec3(0.733306, -2.15188e-09, -0.0741545),
            osg::Vec3(0.758024, -2.15188e-09, -0.205985),

            osg::Vec3(0.885735, -2.15188e-09, -0.0576757),
            osg::Vec3(0.885735, -2.15188e-09, -0.214224),

            osg::Vec3(0.964009, 9.18133e-09, -0.0370773),
            osg::Vec3(1.0464, 9.18133e-09, -0.173027),

            osg::Vec3(1.11232, -2.15188e-09, 0.0123591),
            osg::Vec3(1.12468, 9.18133e-09, -0.164788),
        };

        int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

        osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

        // pass the created vertex array to the points geometry object.
        polyGeom->setVertexArray(vertices);

        // use the shared color array.
        polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);


        // use the shared normal array.
        polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::QUAD_STRIP,0,numCoords));

        printPrimitiveIndices("QUAD_STRIP indices", *polyGeom);
        printPrimitives("QUAD_STRIP vertices", *polyGeom);

        // add the points geometry to the geode.
        geode->addDrawable(polyGeom);
    }

    // create TRIANGLES, TRIANGLE_STRIP and TRIANGLE_FAN all in one Geometry/
    {
        // create Geometry object to store all the vertices and lines primitive.
        osg::Geometry* polyGeom = new osg::Geometry();

        // note, first coord at top, second at bottom, reverse to that buggy OpenGL image..
        osg::Vec3 myCoords[] =
        {
            // TRIANGLES 6 vertices, v0..v5
            // note in anticlockwise order.
            osg::Vec3(-1.12056, -2.15188e-09, -0.840418),
            osg::Vec3(-0.95165, -2.15188e-09, -0.840418),
            osg::Vec3(-1.11644, 9.18133e-09, -0.716827),

            // note in anticlockwise order.
            osg::Vec3(-0.840418, 9.18133e-09, -0.778623),
            osg::Vec3(-0.622074, 9.18133e-09, -0.613835),
            osg::Vec3(-1.067, 9.18133e-09, -0.609715),

            // TRIANGLE STRIP 6 vertices, v6..v11
            // note defined top point first,
            // then anticlockwise for the next two points,
            // then alternating to bottom there after.
            osg::Vec3(-0.160668, -2.15188e-09, -0.531441),
            osg::Vec3(-0.160668, -2.15188e-09, -0.749785),
            osg::Vec3(0.0617955, 9.18133e-09, -0.531441),
            osg::Vec3(0.168908, -2.15188e-09, -0.753905),
            osg::Vec3(0.238942, -2.15188e-09, -0.531441),
            osg::Vec3(0.280139, -2.15188e-09, -0.823939),

            // TRIANGLE FAN 5 vertices, v12..v16
            // note defined in anticlockwise order.
            osg::Vec3(0.844538, 9.18133e-09, -0.712708),
            osg::Vec3(1.0258, 9.18133e-09, -0.799221),
            osg::Vec3(1.03816, -2.15188e-09, -0.692109),
            osg::Vec3(0.988727, 9.18133e-09, -0.568518),
            osg::Vec3(0.840418, -2.15188e-09, -0.506723),

        };

        int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

        osg::Vec3Array* vertices = new osg::Vec3Array(numCoords,myCoords);

        // pass the created vertex array to the points geometry object.
        polyGeom->setVertexArray(vertices);

        // use the shared color array.
        polyGeom->setColorArray(shared_colors.get(), osg::Array::BIND_OVERALL);


        // use the shared normal array.
        polyGeom->setNormalArray(shared_normals.get(), osg::Array::BIND_OVERALL);


        // This time we simply use primitive, and hardwire the number of coords to use
        // since we know up front,
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLES,0,6));
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_STRIP,6,6));
        polyGeom->addPrimitiveSet(new osg::DrawArrays(osg::PrimitiveSet::TRIANGLE_FAN,12,5));

        // polygon stipple
        osg::StateSet* stateSet = new osg::StateSet();
        polyGeom->setStateSet(stateSet);

        #if !defined(OSG_GLES1_AVAILABLE) && !defined(OSG_GLES2_AVAILABLE) && !defined(OSG_GL3_AVAILABLE)
        osg::PolygonStipple* polygonStipple = new osg::PolygonStipple;
        stateSet->setAttributeAndModes(polygonStipple,osg::StateAttribute::OVERRIDE|osg::StateAttribute::ON);
        #endif

        printPrimitiveIndices("Triangles/Strip/Fan indices", *polyGeom);
        printPrimitives("Triangles/Strip/Fan vertices", *polyGeom);

        // add the points geometry to the geode.
        geode->addDrawable(polyGeom);
    }

    // Turn off the lighting on the geode.  This is not required for setting up
    // the geometry.  However, by default, lighting is on, and so the normals
    // above are used to light the geometry.
    // - With lighting turned off, the geometry has the same color
    //   regardless of the angle you view it from.
    // - With lighting turned on, the colors darken as the light moves
    //   away from the normal.
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
                    osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);
    return geode;
}


// define a node callback to animate a transform as a cycle along the y axis, between 0 and 2.0.
class MyTransformCallback : public osg::NodeCallback
{

    public:

        MyTransformCallback(float angularVelocity)
        {
            _angular_velocity = angularVelocity;
        }

        virtual void operator() (osg::Node* node, osg::NodeVisitor* nv)
        {
            osg::MatrixTransform* transform = dynamic_cast<osg::MatrixTransform*>(node);
            if (nv && transform && nv->getFrameStamp())
            {
                double time = nv->getFrameStamp()->getSimulationTime();
                transform->setMatrix(osg::Matrix::translate(0.0f,1.0f+cosf(time*_angular_velocity),0.0f));
            }

            // must continue subgraph traversal.
            traverse(node,nv);

        }

    protected:

        float               _angular_velocity;

};

/// Create a quad that sits behind the geometry from createScene()
/// and shows the image from the OpenGL docs that geometry duplicates.
osg::Node* createBackground()
{

    // we'll create a texture mapped quad to sit behind the Geometry
    osg::ref_ptr<osg::Image> image = osgDB::readRefImageFile("Images/primitives.gif");
    if (!image) return NULL;


    // create Geometry object to store all the vertices and lines primitive.
    osg::Geometry* polyGeom = new osg::Geometry();

    // note, anticlockwise ordering.
    osg::Vec3 myCoords[] =
    {
        osg::Vec3(-1.22908f,0.0f,1.0f),
        osg::Vec3(-1.22908f,0.0f,-1.0f),
        osg::Vec3(1.22908f,0.0f,-1.0f),
        osg::Vec3(1.22908f,0.0f,1.0f)
    };

    int numCoords = sizeof(myCoords)/sizeof(osg::Vec3);

    // pass the created vertex array to the points geometry object.
    polyGeom->setVertexArray(new osg::Vec3Array(numCoords,myCoords));

    osg::Vec4Array* colors = new osg::Vec4Array;
    colors->push_back(osg::Vec4(1.0f,1.0f,1.0f,1.0f));
    polyGeom->setColorArray(colors, osg::Array::BIND_OVERALL);


    // Set the normal in the same way as the color.
    osg::Vec3Array* normals = new osg::Vec3Array;
    normals->push_back(osg::Vec3(0.0f,-1.0f,0.0f));
    polyGeom->setNormalArray(normals, osg::Array::BIND_OVERALL);

    osg::Vec2 myTexCoords[] =
    {
        osg::Vec2(0,1),
        osg::Vec2(0,0),
        osg::Vec2(1,0),
        osg::Vec2(1,1)
    };

    int numTexCoords = sizeof(myTexCoords)/sizeof(osg::Vec2);

    // pass the created tex coord array to the points geometry object,
    // and use it to set texture unit 0.
    polyGeom->setTexCoordArray(0,new osg::Vec2Array(numTexCoords,myTexCoords));

    // we'll use indices and DrawElements to define the primitive this time.
    unsigned short myIndices[] =
    {
        0,
        1,
        3,
        2
    };

    int numIndices = sizeof(myIndices)/sizeof(unsigned short);

    // There are three variants of the DrawElements osg::Primitive, UByteDrawElements which
    // contains unsigned char indices, UShortDrawElements which contains unsigned short indices,
    // and UIntDrawElements which contains ... unsigned int indices.
    // The first parameter to DrawElements is
    polyGeom->addPrimitiveSet(new osg::DrawElementsUShort(osg::PrimitiveSet::TRIANGLE_STRIP,numIndices,myIndices));

    // new we need to add the texture to the Drawable, we do so by creating a
    // StateSet to contain the Texture2D StateAttribute.
    osg::StateSet* stateset = new osg::StateSet;

    // set up the texture.
    osg::Texture2D* texture = new osg::Texture2D;
    texture->setImage(image);

    stateset->setTextureAttributeAndModes(0, texture,osg::StateAttribute::ON);

    polyGeom->setStateSet(stateset);

    printPrimitiveIndices("DrawElementsUShort TRIANGLE_STRIP", *polyGeom);

    // create the Geode (Geometry Node) to contain all our osg::Geometry objects.
    osg::Geode* geode = new osg::Geode();

    // add the points geometry to the geode.
    geode->addDrawable(polyGeom);

    // Turn off the lighting (see note in createScene(), above).
    geode->getOrCreateStateSet()->setMode(GL_LIGHTING,
                    osg::StateAttribute::OFF | osg::StateAttribute::PROTECTED);

    // create a transform to move the background back and forward with.
    osg::MatrixTransform* transform = new osg::MatrixTransform();
    transform->setUpdateCallback(new MyTransformCallback(1.0f));
    transform->addChild(geode);

    return transform;
}

/*
 * ORIGNAL MAIN CODE
 * int main(int, char **)
{
    // create the model
    osg::Group* root = new osg::Group;
    root->addChild( createScene() );
    root->addChild( createBackground() );

    //osgDB::writeNodeFile(*root,"geometry.osgt");

    osgViewer::Viewer viewer;

    // add model to viewer.
    viewer.setSceneData( root );

    return viewer.run();
}
 * */

const int OSG_WIDTH = 1280;
const int OSG_HEIGHT = 960;
const osg::Vec4f FOG_COLOR = osg::Vec4f(0.6f, 0.6f, 0.7f, 1.f);

int main(int, char**)
{
#ifdef _WIN32
    ::SetProcessDPIAware(); // if Windows, for HDPI monitors
#endif // _WIN32

    osgViewer::Viewer viewer;
    osg::DisplaySettings::instance()->setNumMultiSamples(4); // multi sampling is on

    // create the model
    osg::Group* root = new osg::Group;
    root->addChild( createScene() );
    root->addChild( createBackground() );

    //osgDB::writeNodeFile(*root,"geometry.osgt");

    // add model to viewer.
    viewer.setSceneData( root );
    viewer.setUpViewInWindow(100, 100, OSG_WIDTH, OSG_HEIGHT);

    osg::Camera* camera = viewer.getCamera();
    camera->setClearColor(FOG_COLOR);
    camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    viewer.setCameraManipulator( new osgGA::TrackballManipulator);
    viewer.realize();
    while (!viewer.done()){
        //camera->setClearMask(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        viewer.frame();
    }

    return 0;
}
