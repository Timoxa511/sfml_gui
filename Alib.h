//ANIMATION LIB  vLast
#include <sfml/graphics.hpp>
#include <windows.h>
#include  <vector>


typedef sf::Vector2f     Vector;
typedef sf::Vector2i    iVector;

//{not-classes-----------------------------------------------------------------

const sf::IntRect AllTexture (const sf::Texture &tex);


//}
//-----------------------------------------------------------------------------

namespace AL
{/*Classes functions and prototypes*/
//{Prototypes------------------------------------------------------------------

namespace Global
{
const int FLAGVAL = -100500;    //TODO improve sprite.draw() -> sprite.draw (Vector pos = Vector(Fl, Fl)) {if (pos == v (f, f) ...} ...
}

struct Animation
    {

    iVector currentFrame_;

    iVector pixFrameSize_;
    iVector nFrames_;
    iVector pixFrameOffset_;

    //---------------------
    Animation (iVector pixFrameOffset,
               iVector pixFrameSize,
               iVector nFrames);

    sf::IntRect getCurrentFrame () const;
    void update ();
    };


class Sprite
    {
    std::string name_;
    sf::Sprite sprite_;
    Vector pos_;
    sf::RenderWindow* windowp_;

    std::vector <Animation> animations_;
    int animationId_;

    //---------------------
    public:
    Sprite (std::string name);
    Sprite (std::string name,
            const sf::Texture& texture,
            Vector pos,
            sf::RenderWindow* windowp);

    void draw ();

    void setPosition     (const Vector& pos);
    void setTexture      (const sf::Texture& texture);
    void setRenderWindow (sf::RenderWindow* windowp);
    void setAnimationId  (int animationId);


    void addAnimation (Animation animation);
    void addAnimation (iVector frameSize,
                       iVector nFrames,
                       iVector pixFrameOffset);


    sf::RenderWindow*  getWindowp     ();
    const sf::Texture* getTexture     ();
    Animation          getAnimation   (int animationId);
    int                getAnimationId ();
    Vector             getPosition    ();
    };

namespace Global
{
Sprite DefaultSprite ("AJIJIO_ETO_TEXHUK_IIAshA?\nY HAC TYT TEXHUKA HE IIAshET!");
sf::RenderWindow *RenderWindow = nullptr;
}

//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------



//{Sprite::--------------------------------------------------------------------
Sprite::Sprite (std::string name,
                const sf::Texture& texture,
                Vector pos,
                sf::RenderWindow* windowp) :

    name_ (name),
    sprite_ ( sf::Sprite (texture, sf::IntRect()) ),  //empty
    pos_(pos),
    windowp_(windowp),
    animationId_ (0)
    {
    sprite_.setPosition(pos);
    }


//-----------------------------------------------------------------------------
Sprite::Sprite (std::string name) :
    name_ (name),
    sprite_ (),
    pos_ (0, 0),
    windowp_ (nullptr),
    animationId_ (0)
    {}

//=============================================================================
void Sprite::draw ()
    {
    if (sprite_.getTexture())
        {
        if (animations_.size())
            {
            Animation& animation = animations_.at(animationId_);

            sprite_.setTextureRect (animation.getCurrentFrame ());
            animation.update();
            }
        else                                                                         //SOO AMAZING FN IT CALS ITSELF BUT WORKS
            {
            sprite_.setTextureRect ( AllTexture(*sprite_.getTexture()) );
            }

        windowp_->draw(sprite_);
        }
    else
        {
        setTexture  (*AL::Global::DefaultSprite.getTexture());
        addAnimation (AL::Global::DefaultSprite.getAnimation(0));
        setRenderWindow (AL::Global::RenderWindow);
        draw();
        }
    }



//-----------------------------------------------------------------------------
void Sprite::setPosition (const Vector& pos)
    {
    pos_ = pos;
    sprite_.setPosition (pos_);
    }


//-----------------------------------------------------------------------------
void Sprite::setTexture (const sf::Texture& texture)
    {
    sprite_.setTexture (texture);
    }


//-----------------------------------------------------------------------------
void Sprite::setRenderWindow (sf::RenderWindow* windowp)
    {
    windowp_ = windowp;
    }


//-----------------------------------------------------------------------------
void Sprite::setAnimationId (int animationId)
    {
    animationId_ = animationId;
    }




//-----------------------------------------------------------------------------
void Sprite::addAnimation (Animation animation)
    {
    animations_.push_back (animation);
    }


//-----------------------------------------------------------------------------
void Sprite::addAnimation (iVector pixFrameSize,
                           iVector nFrames,
                           iVector pixFrameOffset)
    {
    animations_.push_back (Animation (pixFrameSize,
                                      nFrames,
                                      pixFrameOffset) );
    }




//-----------------------------------------------------------------------------
const sf::Texture* Sprite::getTexture ()
    {
    return sprite_.getTexture();
    }


//-----------------------------------------------------------------------------
Vector Sprite::getPosition ()
    {
    return pos_;
    }


//-----------------------------------------------------------------------------
sf::RenderWindow* Sprite::getWindowp ()
    {
    return windowp_;
    }


//-----------------------------------------------------------------------------
int Sprite::getAnimationId ()
    {
    return animationId_;
    }


//-----------------------------------------------------------------------------
Animation Sprite::getAnimation (int animationId)
    {
    return animations_.at(animationId);
    }


//}
//-----------------------------------------------------------------------------


//{Animation::-----------------------------------------------------------------
Animation::Animation (iVector pixFrameSize,
                      iVector nFrames,
                      iVector pixFrameOffset) :
    currentFrame_ ( iVector (0, 0) ),      //1st frame 1st row /cap/

    pixFrameSize_ (pixFrameSize),
    nFrames_ (nFrames),
    pixFrameOffset_ (pixFrameOffset)
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


//-----------------------------------------------------------------------------
sf::IntRect Animation::getCurrentFrame () const
    {
    return sf::IntRect (currentFrame_.x * pixFrameSize_.x,
                        currentFrame_.y * pixFrameSize_.y,
                                          pixFrameSize_.x,
                                          pixFrameSize_.y);
    }
//}
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------
}

//{Functions-------------------------------------------------------------------

int GUImain ();

int main()
    {
    sf::Texture defTexture;
    bool    load_succeeded = defTexture.loadFromFile("defTexture.jpg");
    assert (load_succeeded);

    AL::Global::DefaultSprite.setTexture (defTexture);
    AL::Global::DefaultSprite.setRenderWindow(AL::Global::RenderWindow);
    AL::Global::DefaultSprite.addAnimation (iVector (defTexture.getSize().x/2, defTexture.getSize().y),
                                            iVector (1, 2),
                                            iVector (0, 0) );

    //GUImain ();
    sf::RenderWindow win (sf::VideoMode (1000, 800), "test" );
    AL::Global::RenderWindow = &win;
    sf::Texture t;
    t.loadFromFile ("example.jpg");

    AL::Sprite s ("imya", t, Vector (400, 400), &win);
    s.addAnimation(iVector (128, 128), iVector (8, 8), iVector (0, 0));

    while(!GetAsyncKeyState(VK_SPACE))
        {
        win.clear();
        s.draw();

        win.display();
        Sleep(20);
        }

    }
#define main GUImain

 /* example-test
    sf::RenderWindow win (sf::VideoMode (1000, 800), "test" );
    sf::Texture t;
    t.loadFromFile ("example.jpg");

    AL::Sprite s ("imya", t, Vector (400, 400), &win);
    s.addAnimation(iVector (128, 128), iVector (8, 8), iVector (0, 0));

    while(!GetAsyncKeyState(VK_SPACE))
        {
        win.clear();
        s.draw();

        win.display();
        Sleep(20);
        }
       */
//-----------------------------------------------------------------------------

Vector operator / (const Vector& lvalue, int rvalue)
    {
    return Vector (lvalue.x / rvalue, lvalue.y / rvalue);
    }

const sf::IntRect AllTexture (const sf::Texture &tex)
    {
    auto size2u = tex.getSize();
    Vector  size2 (size2u);
    return sf::IntRect (0, 0, size2.x, size2.y);
    }


Vector& operator /= (Vector& lvalue, const Vector& rvalue)
    {
    lvalue.x /= rvalue.x;
    lvalue.y /= rvalue.y;

    return lvalue;
    }

Vector operator / (const Vector& lvalue, const Vector& rvalue)
    {
    Vector ret = lvalue;
    ret /= rvalue;
    return ret;
    }


bool operator == (const Vector& lvalue, const Vector& rvalue)
    {
    return (lvalue.x == rvalue.x && lvalue.y == rvalue.y);
    }
//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------





//TODO  многоанимационный спрайт, то бишь массив спрайтов => слои, добавить слой()  добавить анимацию на слой()   update()/пересчитывать относительные координаты спрайтов/
                                                                                //



