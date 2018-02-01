//ANIMATION LIB  v0     одинкаовое колво кадров во всех анимациях, одинаковый размер всех кадров,
#include <sfml/graphics.hpp>
#include <windows.h>
#include "GUI.h"
//namsp


//{Prototypes------------------------------------------------------------------


typedef sf::Vector2f     Vector;

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

struct Sprite
    {
    sf::Sprite sprite_;

    Vector pos_;

    int   animation_;
    int   frame_;

    sf::Vector2i frameSize_;
    int          nFrames_;
    int          nAnimations_;

    sf::RenderWindow* windowp_;

    //---------------------
    Sprite (const sf::Texture& texture,
            Vector pos,
                sf::Vector2i frameSize,
                int  nFrames     /*= 1*/,
                int  nAnimations /*= 1*/,
                    sf::RenderWindow* windowp = nullptr);
    void draw ();
    void setAnimation(int animation);
    };

//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------


//{Sprite:::-------------------------------------------------------------------
Sprite::Sprite (const sf::Texture& texture,
                Vector pos,
                    sf::Vector2i frameSize,
                    int  nFrames,
                    int  nAnimations,
                     sf::RenderWindow* windowp) :

    sprite_ ( sf::Sprite (texture, sf::IntRect())),  //empty
    pos_(pos),
                                 //     ^
    animation_(0),    //top            /|\ .
    frame_    (0),    //left            |
                              //        |
    frameSize_   (frameSize),  //width + height
    nFrames_     (nFrames),
    nAnimations_ (nAnimations),

    windowp_(windowp)
    {
    sprite_.setPosition(pos);
    }

//=============================================================================
void Sprite::draw ()
    {
    sprite_.setTextureRect(sf::IntRect (frame_ * frameSize_.x,   animation_ * frameSize_.y,
                                        frameSize_.x,        frameSize_.y)   );
    windowp_->draw(sprite_);

    frame_ = ++frame_ % nFrames_;
    }



//-----------------------------------------------------------------------------
void Sprite::setAnimation (int animation)
    {
    frame_     = 0;
    animation_ = animation;

    sprite_.setTextureRect (sf::IntRect (frame_ * frameSize_.x,   animation_ * frameSize_.y,
                                         frameSize_.x,        frameSize_.y)   );

    }


//}
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------

//{Functions-------------------------------------------------------------------

int main()
    {
    Window win ("test", Vector (1000, 800),Vector (200, 200));


    sf::Texture t;
    t.loadFromFile ("example.jpg");

    Sprite s (t, Vector (400, 400), sf::Vector2i (128, 128), 8, 8, &win.getWindow());

    int time = 0;

    while(win.run())
        {

        s.draw();

        if (++time % 8 == 0) s.setAnimation ((time/8) % 8);


        Sleep(100);
        }

    }

//-----------------------------------------------------------------------------


/*Vector operator / (const Vector& lvalue, int rvalue)
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

           */
//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

