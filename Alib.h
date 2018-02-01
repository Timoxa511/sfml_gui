//ANIMATION LIB  v1
#include <sfml/graphics.hpp>
#include <windows.h>
#include  <vector>


typedef sf::Vector2f     Vector;
typedef sf::Vector2i    iVector;

namespace AL
{/*Classes functions and prototypes*/
//{Prototypes------------------------------------------------------------------

struct Animation
    {

    iVector currentFrame_;

    iVector frameSize_;
    iVector nFrames_;
    iVector animationBeginingPos_;

    //---------------------
    Animation (iVector animationBeginingPos,
               iVector frameSize,
               iVector nFrames);
    void update ();
    };


class Sprite
    {
    sf::Sprite sprite_;
    Vector pos_;
    sf::RenderWindow* windowp_;

    std::vector <Animation> animations_;
    int animation_;

    //---------------------
    public:
    Sprite (const sf::Texture& texture,
            Vector pos,
            sf::RenderWindow* windowp);

    void draw ();

    void setPosition (const Vector& pos);
    void setAnimation (int animation);
    void addAnimation (iVector frameSize            = iVector (1, 1),
                       iVector nFrames              = iVector (1, 1),
                       iVector animationBeginingPos = iVector (0, 0)  );


    iVector getTextureSize();
     Vector getPosition();
    };

//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------



//{Sprite::--------------------------------------------------------------------
Sprite::Sprite (const sf::Texture& texture,
                Vector pos,
                sf::RenderWindow* windowp) :

    sprite_ ( sf::Sprite (texture, sf::IntRect()) ),  //empty
    pos_(pos),
    windowp_(windowp)
    {
    sprite_.setPosition(pos);                                            //TODO +constructor {default animation}
    }



//=============================================================================
void Sprite::draw ()
    {
    Animation& animation = animations_.at(animation_);
    sprite_.setTextureRect(    sf::IntRect (animation.currentFrame_.x * animation.frameSize_.x,
                                            animation.currentFrame_.y * animation.frameSize_.y,
                                                                        animation.frameSize_.x,
                                                                        animation.frameSize_.y)   );
    windowp_->draw(sprite_);

    animation.update();
    }


//-----------------------------------------------------------------------------
void Sprite::setPosition (const Vector& pos)
    {
    pos_ = pos;
    sprite_.setPosition (pos_);
    }



//-----------------------------------------------------------------------------
void Sprite::setAnimation (int animation)
    {
    animation_ = animation;
    }



//-----------------------------------------------------------------------------
void Sprite::addAnimation (iVector frameSize,
                           iVector nFrames,
                           iVector animationBeginingPos)
    {
    animations_.push_back (Animation ( (frameSize == iVector (1, 1) )? getTextureSize() : frameSize,
                                        nFrames,
                                        animationBeginingPos) );
    }



//-----------------------------------------------------------------------------
iVector Sprite::getTextureSize ()
    {
    return (iVector) sprite_.getTexture()->getSize();
    }




//-----------------------------------------------------------------------------
Vector Sprite::getPosition ()
    {
    return pos_;
    }



//}
//-----------------------------------------------------------------------------


//{Animation::-----------------------------------------------------------------
Animation::Animation (iVector frameSize,
                      iVector nFrames,
                      iVector animationBeginingPos) :
    currentFrame_ ( iVector (0, 0) ),      //1st frame 1st row /cap/

    frameSize_ (frameSize),
    nFrames_ (nFrames),
    animationBeginingPos_ (animationBeginingPos)
    {

    }


//=============================================================================
void Animation::update ()
    {
    currentFrame_.x++;
                                       //плюс тот факт, что это так или не так
    currentFrame_.y = (currentFrame_.y + (currentFrame_.x >= nFrames_.x)) % nFrames_.y;
    currentFrame_.x =  currentFrame_.x % nFrames_.x;
    }



//}
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------
}

//{Functions-------------------------------------------------------------------

/*int main()
    {
    sf::RenderWindow win (sf::VideoMode (1000, 800), "test" );
    sf::Texture t;
    t.loadFromFile ("example.jpg");

    Sprite s (t, Vector (400, 400), &win);
    s.addAnimation(iVector (0, 0), iVector (128, 128), iVector (8, 8));
    s.setAnimation(0);
    int time = 0;

    while(!GetAsyncKeyState(VK_SPACE))
        {
        win.clear();
        s.draw();

        if (++time % 8 == 0) s.setAnimation ((time/8) % 8);

        win.display();
        Sleep(100);
        }

    }      */

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

