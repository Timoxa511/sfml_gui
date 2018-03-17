#include <sfml/graphics.hpp>

typedef sf::Vector2f     Vector;
typedef sf::Vector2i    iVector;

//{operators-------------------------------------------------------------------

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
    return (lvalue.x == rvalue.x && lvalue.y == rvalue.y);
    }


//-----------------------------------------------------------------------------
const sf::IntRect AllTexture (const sf::Texture &tex)
    {
    auto size2u = tex.getSize();
    Vector  size2 (size2u);
    return sf::IntRect (0, 0, size2.x, size2.y);
    }


//}
//-----------------------------------------------------------------------------

