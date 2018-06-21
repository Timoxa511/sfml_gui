#include <sfml/graphics.hpp>

typedef sf::Vector2f     Vector;
typedef sf::Vector2i    iVector;



//{prototypes------------------------------------------------------------------

bool isEqual (float pupsen, float vupsen);

Vector  operator /  (const Vector& lvalue, int rvalue);
Vector& operator /= (Vector& lvalue, const Vector& rvalue);
Vector  operator /  (const Vector& lvalue, const Vector& rvalue);
bool    operator == (const Vector& lvalue, const Vector& rvalue);

const sf::IntRect AllTexture (const sf::Texture &tex);

//}
//-----------------------------------------------------------------------------

//{functions-------------------------------------------------------------------

Vector operator / (const Vector& lvalue, int rvalue)
    {
    return Vector (lvalue.x / float (rvalue), lvalue.y / float (rvalue));
    }

//-----------------------------------------------------------------------------
Vector& operator /= (Vector& lvalue, const Vector& rvalue)
    {
    lvalue.x /= rvalue.x;
    lvalue.y /= rvalue.y;

    return lvalue;
    }


//-----------------------------------------------------------------------------
Vector operator / (const Vector& lvalue, const Vector& rvalue)
    {
    Vector ret = lvalue;
    ret /= rvalue;
    return ret;
    }


//-----------------------------------------------------------------------------
bool operator == (const Vector& lvalue, const Vector& rvalue)
    {
    return ( isEqual (lvalue.x, rvalue.x) && isEqual (lvalue.y, rvalue.y) );
    }


//-----------------------------------------------------------------------------
const sf::IntRect AllTexture (const sf::Texture &tex)
    {
    auto size = tex.getSize();
    return sf::IntRect (0, 0, size.x, size.y);
    }


bool isEqual (float pupsen, float vupsen)
    {
    return (fabs (pupsen - vupsen) < DBL_EPSILON);
    }

//}
//-----------------------------------------------------------------------------


