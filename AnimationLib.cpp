//ANIMATION LIB  v0     одинкаовое колво кадров во всех анимациях, одинаковый размер всех кадров,
#include <sfml/graphics.hpp>
//namsp


//{Prototypes------------------------------------------------------------------


typedef sf::Vector2f     Vector;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct Sprite
    {
    sf::Sprite sprite_;

    sf::Vector2i frameSize_;
    int  frame_;
    int  nFrames_;

    //---------------------
    Sprite (sf::Tecture texture,
               sf::Vector2i frameSize,
               int  nFrames);
    void draw ();
    void update ();
    void setAnimation(int animation);
    };

//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------


//{Sprite:::-------------------------------------------------------------------
Sprite::Sprite (sf::Tecture& texture,
                          sf::Vector2i frameSize,
                          int  nFrames) :
    sprite_ ( sf::Sprite (texture, sf::IntRect())),  //empty

    frameSize_ (frameSize),
    frame_ (-1),
    nFrames_ (nFrames),
    {}

//=============================================================================
void Sprite::draw ()
    {
    sprite_.draw();
    }



//-----------------------------------------------------------------------------
void Sprite::update ()
    {
    frame_ = ++frame_ % nFrames_;
    sprite_.setTextureRect(     frame_ * frameSize_.x,   animation * frameSize_.y,
                                frameSize_.x,        frameSize_.y   );
    }



//-----------------------------------------------------------------------------
void Sprite::setAnimation (int animation)
    {
    frame_     = 0;
    animation_ = animation;

    sprite_.setTextureRect (     frame_ * frameSize_.x,   animation_ * frameSize_.y,
                                 frameSize_.x,        frameSize_.y   );

    }


//}
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------

//{Functions-------------------------------------------------------------------

int main()
    {


    }

//-----------------------------------------------------------------------------


Vector operator / (const Vector& lvalue, int rvalue)
    {
    return Vector (lvalue.x / rvalue, lvalue.y / rvalue);
    }

const sf::IntRect AllTexture (const sf::Texture &tex)
    {

    auto size2u = tex.getSize();
    Vector size2 (size2u);
    return sf::IntRect (0, 0, size2.x, size2.y);
    }

const sf::IntRect The_Half_TextureRect (const sf::Texture &tex)
    {

    auto size2u = tex.getSize();
    Vector size2 (size2u);
    return sf::IntRect (0, 0, size2.x/2, size2.y);
    }


//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

