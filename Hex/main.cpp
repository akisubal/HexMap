//
//  main.cpp
//  Hex
//
//  Created by akisubal on 2013/01/02.
//  Copyright (c) 2013年 akisubal. All rights reserved.
//



#include <iostream>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iterator>
#include <list>
#define _USE_MATH_DEFINES
#include <cmath>
#include <ctime>
#include <glew.h>
#include <GLUT/glut.h>
#include <cassert>

#include "HexChip.h"
#include "HexMapPosition.h"
#include "HexMap.h"

HexMapPosition::Neighbor GetNeighbor(char c)
{
    switch (c)
    {
        case 'g':
            return HexMapPosition::LeftUp;
        case 'c':
            return HexMapPosition::RightUp;
        case 'd':
            return HexMapPosition::Left;
        case 'b':
            return HexMapPosition::LeftDown;
        case 'm':
            return HexMapPosition::RightDown;
        case 't':
            return HexMapPosition::Right;
        default:
            return HexMapPosition::Left;
    }
}


class VertexBuffer
{
public:
    VertexBuffer()
    :m_handle_index(0)
    ,m_prev_handle_index(0)
    {}
    
    ~VertexBuffer()
    {
        Finalize();
    }
    
    void Initialize(const void* vertices, size_t size)
    {
        if (m_handle_index != 0) { return; }
        GLint h(0);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &h);
        assert(0 <= h);
        
        glGenBuffers(1, &m_handle_index);
        glBindBuffer(GL_ARRAY_BUFFER, m_handle_index);
        glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);
        glBindBuffer(GL_ARRAY_BUFFER, h);
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Finalize()
    {
        if (m_handle_index == 0) { return; }
        glDeleteBuffers(1, &m_handle_index);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Bind()
    {
        GLint h(0);
        glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &h);
        assert(0 <= h);
        
        m_prev_handle_index = h;
        glBindBuffer(GL_ARRAY_BUFFER, m_handle_index);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Unbind()
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_prev_handle_index);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    class Lock
    {
    public:
        Lock(VertexBuffer& v)
        :m_vertex_buffer(v)
        {
            m_vertex_buffer.Bind();
        }
        
        ~Lock()
        {
            m_vertex_buffer.Unbind();
        }
    private:
        VertexBuffer& m_vertex_buffer;
    };
    
private:
    GLuint m_handle_index;
    GLuint m_prev_handle_index;
};

class IndexBuffer
{
public:
    IndexBuffer()
    :m_handle_index(0)
    ,m_prev_handle_index(0)
    {}
    
    ~IndexBuffer()
    {}
    
    void Initialize(const void* indices, size_t size)
    {
        if (m_handle_index != 0) { return; }
        GLint h(0);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &h);
        assert(0 <= h);
        
        glGenBuffers(1, &m_handle_index);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle_index);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, size, indices, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, h);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Finalize()
    {
        if (m_handle_index == 0) { return; }
        glDeleteBuffers(1, &m_handle_index);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Bind()
    {
        GLint h(0);
        glGetIntegerv(GL_ELEMENT_ARRAY_BUFFER_BINDING, &h);
        assert(0 <= h);
        m_prev_handle_index = h;
        
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_handle_index);        
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    void Unbind()
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_prev_handle_index);
        
        assert(glGetError() == GL_NO_ERROR);
    }
    
    
    class Lock
    {
    public:
        Lock(IndexBuffer& i)
        :m_index_buffer(i)
        {
            m_index_buffer.Bind();
        }
        
        ~Lock()
        {
            m_index_buffer.Unbind();
        }
    private:
        IndexBuffer& m_index_buffer;
    };
private:
    GLuint m_handle_index;
    GLuint m_prev_handle_index;
};

class HexPrimitive
{
public:
    HexPrimitive()
    :m_vertex_buffer()
    ,m_index_buffer()
    {
    }
    
    ~HexPrimitive()
    {}
    
    void Initialize()
    {
        m_vertex_buffer.Initialize(vertex, sizeof(vertex));
        m_index_buffer.Initialize(indices, sizeof(indices));
    }
    
    void Finalize()
    {
        
    }
    
    struct Color
    {
        Color()
        :r(0.0f), g(0.0f), b(0.0f)
        {}
        
        Color(GLfloat r_, GLfloat g_, GLfloat b_)
        :r(r_), g(g_), b(b_)
        {}
        GLfloat r, g, b;
    };
    
    void Draw(Color c = Color())
    {
        VertexBuffer::Lock v_lock = VertexBuffer::Lock(m_vertex_buffer);
        IndexBuffer::Lock i_lock = IndexBuffer::Lock(m_index_buffer);
        
        glInterleavedArrays(GL_V3F, 0, NULL);
        
        assert(glGetError() == GL_NO_ERROR);
        
        glColor3f(c.r, c.g, c.b);
        glDrawElements(GL_TRIANGLE_FAN, sizeof(indices) / sizeof(indices[0]), GL_UNSIGNED_INT, NULL);
        assert(glGetError() == GL_NO_ERROR);
    }
    
    struct Vertex
    {
        GLfloat x, y, z;
    };
    
    static const Vertex vertex[7];
    static const GLuint indices[8];
    
private:
    VertexBuffer m_vertex_buffer;
    IndexBuffer m_index_buffer;    
};

/// 頂点情報
const HexPrimitive::Vertex HexPrimitive::vertex[] =
{
    { 0.0f,           0.0f, 0.0f },
    
    { 0.0f,           1.0f, 0.0f },
    { 0.866025404f,   0.5f, 0.0f },
    { 0.866025404f,  -0.5f, 0.0f },
    { 0.0f,          -1.0f, 0.0f },
    { -0.866025404f, -0.5f, 0.0f },
    { -0.866025404f,  0.5f, 0.0f },
};

/// インデックス
const GLuint HexPrimitive::indices[] =
{
    0, 1, 2, 3, 4, 5, 6, 1
};


struct Translation
{
    Translation(GLfloat x_, GLfloat y_, GLfloat z_)
    :x(x_)
    ,y(y_)
    ,z(z_)
    {}
    
    Translation()
    :x(0.0f)
    ,y(0.0f)
    ,z(0.0f)
    {}
    
    GLfloat x,y,z;
};

Translation GetTranslationFromHexMapPosition(const HexMapPosition& pos)
{
    return Translation(2*pos.X() + ((pos.Y() % 2 == 1) ? 1 : 0), -2*pos.Y(), 0);
}

static const HexPrimitive::Color s_hex_colors[HexChip::Count] =
{
    HexPrimitive::Color(1.0f, 1.0f, 1.0f),
    HexPrimitive::Color(0.5f, 0.5f, 0.5f),
};



class Stroke
{
public:
    struct Position
    {
        Position()
        :x(0)
        ,y(0)
        {}
        
        Position(int x_, int y_)
        :x(x_)
        ,y(y_)
        {}
        
        int x;
        int y;
    };
    
    /// 角度 時計回りで12時が0
    struct Angle
    {
        Angle()
        :m_angle(0.0f)
        {}
        
        explicit Angle(float a)
        :m_angle(a)
        {}
        
        operator float()
        {
            return m_angle;
        }
        
        float m_angle;
    };
    
    /// ax+by+c=0の形の直線
    class Line
    {
    public:        
        Line(const Position& start, const Position& end)
        :a(0.0f)
        ,b(0.0f)
        ,c(0.0f)
        {
            if (end.x == start.x) {
                a = 1.0f;
                b = 0.0f;
                c = end.x;
            }
            else {
                a = static_cast<float>(end.y - start.y) / static_cast<float>(end.x - start.x);
                b = -1.0f;
                c = start.y - a * start.x;
            }
        }
        
        
        float Distance(const Position& pos) const
        {
            return std::abs(a * pos.x + b * pos.y + c) / std::sqrt(a*a + b*b);
        }
        
    private:
        float a;
        float b;
        float c;
    };
    

    
    Stroke()
    :m_history()
    ,m_elapsed_time(0)
    {}
    
    ~Stroke()
    { }
    
    void Add(const Position& pos)
    {
        m_history.push_back(pos);
    }
    
    void Clear()
    {
        m_history.clear();
    }
    
    size_t RelayCount()
    {
        return m_history.size();
    }
    
    Angle Slant()
    {
        const Position& start = m_history.front();
        const Position& end   = m_history.back();
        
        return (start.x != end.x) ? Angle(180.0f - 180.0f * M_1_PI * atan2(end.x - start.x, end.y - start.y))
        : Angle((end.y < start.y) ? 0.0f : 180.0f);
    }
    
    float LengthCube() const
    {
        typedef std::list<Position>::const_iterator Iterator;
        float length(0.0f);
        
        for (Iterator itr = m_history.begin(); itr != m_history.end(); ++itr)
        {
            Iterator next = itr;
            ++next;
            if (next == m_history.end()) { break; }
            const float dif_x = (*next).x - (*itr).x;
            const float dif_y = (*next).y - (*itr).y;
            
            length += dif_x * dif_x + dif_y * dif_y;            
        }
        
        return length;        
    }
    
    double ElapsedTime() const { return m_elapsed_time; }
    void SetElapsedTime(double e) { m_elapsed_time = e; }
    
    
private:
    std::list<Position> m_history;
    double m_elapsed_time;
};

HexMapPosition::Neighbor GetNeighbor(Stroke::Angle a)
{
    if (a < 60.0f) {
        return HexMapPosition::RightUp;
    }
    else if (a < 120.0f) {
        return HexMapPosition::Right;
    }
    else if (a < 180.0f) {
        return HexMapPosition::RightDown;
    }
    else if (a < 240.0f) {
        return HexMapPosition::LeftDown;
    }
    else if (a < 300.0f) {
        return HexMapPosition::Left;
    }
    else {
        return HexMapPosition::LeftUp;
    }
}


class StrokeDetector
{
public:
    typedef Stroke::Position Position;
    
    StrokeDetector()
    :m_is_active(false)
    ,m_start_time(0)
    ,m_stroke()
    {}
    
    ~StrokeDetector()
    {}
    
    void Start(Position pos = Position())
    {
        m_is_active = true;
        time(&m_start_time);
        
        m_stroke.Clear();
        m_stroke.Add(pos);
    }
    
    Stroke End(Position pos = Position())
    {
        m_stroke.Add(pos);
        m_is_active = false;
        
        time_t end_time;
        time(&end_time);
        m_stroke.SetElapsedTime(difftime(end_time, m_start_time));
        return m_stroke;
    }
    
    void Set(Position pos = Position())
    {
        if (!m_is_active) { return; }
        m_stroke.Add(pos);
    }
    
    bool IsActive() const { return m_is_active; }
private:
    bool m_is_active;
    time_t m_start_time;
    Stroke m_stroke;
};

static HexPrimitive hex;

static HexMap<HexChip, 5, 5> hex_map;
static HexMapPosition pos(1,1);
static StrokeDetector stroke_detector;



void displayFunc()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
    // マップ
    for (HexMapPositionIterator<5,5> itr(HexMapPositionIterator<5,5>::begin());
         itr != HexMapPositionIterator<5,5>::end();
         ++itr)
    {
        glPushMatrix();
        const Translation trans = GetTranslationFromHexMapPosition(*itr);
        glTranslatef(trans.x, trans.y, trans.z);
        hex.Draw(s_hex_colors[hex_map[*itr]]);
        glPopMatrix();
    }
    
    // プレイヤ
    glPushMatrix();
    const Translation t = GetTranslationFromHexMapPosition(pos);
    glTranslated(t.x, t.y, t.z);
    hex.Draw(HexPrimitive::Color(1.0f, 0.0f, 0.0f));
    glPopMatrix();
    glFlush();
    
    glutSwapBuffers();
    
    assert(glGetError() == GL_NO_ERROR);
}

void keyboardFunc(unsigned char key, int, int)
{
    switch (key) {
        case '\033':
            exit(EXIT_SUCCESS);
            break;
        default:
            break;
    }
    
    const HexMapPosition::Neighbor neighbor = GetNeighbor(key);
    if (IsEntriable(hex_map, pos.GetNeighbor(neighbor))) {
        pos = pos.GetNeighbor(neighbor);
    }
}

void idleFunc()
{
    glutPostRedisplay();
}

void reshapeFunc(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    
    glOrtho(-w / 30.0, w / 30.0, -h / 30.0, h / 30.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    
    assert(glGetError() == GL_NO_ERROR);
}

void mouseFunc(int button, int state, int x, int y)
{
    
    if (state == GLUT_DOWN)
    {
        stroke_detector.Start(Stroke::Position(x,y));
        return;
    }
    
    if (state == GLUT_UP)
    {
        Stroke stroke = stroke_detector.End(Stroke::Position(x,y));
        
        std::cout << stroke.ElapsedTime() << std::endl;

        const float length_cube = stroke.LengthCube();
        if (length_cube < 100) { return; }
        if (5000 < length_cube) { return; }
        
        const HexMapPosition::Neighbor neighbor = GetNeighbor(stroke.Slant());
        if (IsEntriable(hex_map, pos.GetNeighbor(neighbor))) {
            pos = pos.GetNeighbor(neighbor);
        }
        return;
    }
}

void motionFunc(int x, int y)
{
    stroke_detector.Set(Stroke::Position(x,y));
    
}

int main(int argc, char* argv[])
{
    hex_map[HexMapPosition(2,3)] = HexChip::NoEntry;
#if 0
    const HexMap<HexMapPosition, 5, 5> path_map = GeneratePathMap(hex_map, pos);
    
    HexMap<int, 5, 5> distance_map;
    std::transform(HexMapPositionIterator<5,5>::begin()
                   ,HexMapPositionIterator<5,5>::end()
                   ,distance_map.begin()
                   ,CalcDistance<5, 5>(path_map, pos)
                   );
    
    std::cout << distance_map <<std::endl;
    
    char c;		
    while (std::cin >> c) {
        const HexMapPosition::Neighbor neighbor = GetNeighbor(c);
        hex_map[pos].SetType(HexChip::Standard);
        if (IsEntriable(hex_map, pos.GetNeighbor(neighbor))) {
            pos = pos.GetNeighbor(neighbor);
        }
        hex_map.At(pos).SetType(HexChip::NoEntry);
        
        
        std::cout << neighbor << std::endl;
        std::cout << hex_map << std::endl;
        std::cout << "cursor: " << pos << std::endl;
    }
#endif
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
    
    glutInitWindowSize(720, 480);
    glutCreateWindow("Hex Map");
    
    glutDisplayFunc(displayFunc);
    glutKeyboardFunc(keyboardFunc);
    glutIdleFunc(idleFunc);
    glutReshapeFunc(reshapeFunc);
    glutMouseFunc(mouseFunc);
    glutMotionFunc(motionFunc);
    
    if (glewInit() != GLEW_OK) { return EXIT_FAILURE; }
    
    hex.Initialize();

    glClearColor(0.0, 0.0, 0.0, 1.0);
    
    assert(glGetError() == GL_NO_ERROR);
    glutMainLoop();
    return 0;
 }

