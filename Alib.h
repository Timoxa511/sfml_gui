//ANIMATION LIB  vLast
#include <sfml/graphics.hpp>
#include "AlibOperators.hpp"
#include <windows.h>
#include  <vector>

#define DEBUG

namespace AL
{/*Classes functions and prototypes*/
//{Prototypes------------------------------------------------------------------


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


struct    ISprite
    {

    //---------------------
    virtual void draw () = 0;

    virtual void setPosition     (const Vector& pos) = 0;
    virtual void setTexture      (const sf::Texture& texture) = 0;
    virtual void setRenderWindow (sf::RenderWindow* windowp) = 0;

    virtual sf::RenderWindow*  getWindowp     () = 0;
    virtual const sf::Texture* getTexture     () = 0;
    virtual Vector             getPosition    () = 0;
    };


class      Sprite : public ISprite
    {
    std::string name_;
    sf::Sprite sprite_;
    Vector pos_;
    sf::RenderWindow* windowp_;

    std::vector <Animation> animations_;
    int animationId_;

    //---------------------
    public:
             Sprite (const Sprite& sprite);
    explicit Sprite (std::string name);
             Sprite (std::string name,
                     const sf::Texture& texture,
                     Vector pos,
                     sf::RenderWindow* windowp);

    virtual void draw () override;
            void draw (Vector offset);

    virtual void setPosition     (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture& texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;
            void setAnimationId  (int animationId);


            void addAnimation (Animation animation);
            void addAnimation (iVector frameSize,
                               iVector nFrames,
                               iVector pixFrameOffset);


    virtual sf::RenderWindow*  getWindowp     ()  override;
    virtual const sf::Texture* getTexture     ()  override;
    virtual Vector             getPosition    ()  override;
            int                getAnimationId ();
            Animation          getAnimation   (int animationId);


    Sprite& operator = (const Sprite& that) = delete;
    };


class LayerSprite : public ISprite
    {
    std::string name_;
    Vector pos_;
    sf::RenderWindow* windowp_;

    std::vector <Sprite> layers_;
    //-------------------
    public:
             LayerSprite (const LayerSprite& sprite);
    explicit LayerSprite (std::string name);
             LayerSprite (std::string name,
                          const sf::Texture& texture,
                          Vector pos,
                          sf::RenderWindow* windowp);

    virtual void draw () override;

    virtual void setPosition     (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture& texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;

    virtual sf::RenderWindow*  getWindowp     ()              override;
    virtual const sf::Texture* getTexture     ()              override;
    virtual Vector             getPosition    ()              override;

        private:
            void deleteLayers();  //itl caus trablz when u try to create layers after deleting it as will be no tex
        public:
            void addLayer (Vector LocalPos, Animation animation);
            AL::Sprite* getLayer (int Id);

    LayerSprite& operator = (const Sprite& that) = delete;
    };



namespace Global
{
const int FLAGVAL = -100500;    //TODO improve sprite.draw() -> sprite.draw (Vector pos = Vector(Fl, Fl)) {if (pos == v (f, f) ...} ...
Sprite DefaultSprite ("AJIJIO_ETO_TEXHUK_IIAshA?\nY HAC TYT TEXHUKA HE IIAshET!");
sf::RenderWindow *RenderWindow = nullptr;
}


//}
//-----------------------------------------------------------------------------

//=======================================================================================


//{Classes---------------------------------------------------------------------



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


//{Sprite::--------------------------------------------------------------------
Sprite::Sprite (std::string name,
                sf::RenderWindow* windowp,
                sf::Texture* texture,
                Vector pos) :

    name_ (name),
    sprite_ ( sf::Sprite (texture, sf::IntRect()) ),  //empty
    pos_(pos),
    windowp_(windowp),
    animations_ (),
    animationId_ (0)
    {
    assert (windowp);
    sprite_.setPosition(pos);
    }


//-----------------------------------------------------------------------------
Sprite::Sprite (std::string name) :
    name_ (name),
    sprite_ (),
    pos_ (0, 0),
    windowp_ (nullptr),
    animations_ (),
    animationId_ (0)
    {}

//-----------------------------------------------------------------------------
Sprite::Sprite (const Sprite& sprite) :
    name_        (sprite.name_),
    sprite_      (sprite.sprite_),
    pos_         (sprite.pos_),
    windowp_     (sprite.windowp_),
    animations_  (sprite.animations_),
    animationId_ (sprite.animationId_)
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
            animation.update();                                                    //built-in update
            }
        else                                                                         //SOO AMAZING FN IT CALS ITSELF BUT WORKS
            {
            sprite_.setTextureRect ( AllTexture(*sprite_.getTexture()) );
            }

        windowp_->draw(sprite_);
        }
    else
        {
        addAnimation (AL::Global::DefaultSprite.getAnimation(0));
        setAnimationId (int (animations_.size() - 1));
        setRenderWindow (AL::Global::RenderWindow);
        setTexture  (*AL::Global::DefaultSprite.getTexture());
        draw();
        }
    }



//-----------------------------------------------------------------------------
void Sprite::draw (Vector offset)
    {
    sprite_.move(offset);

    draw ();

    sprite_.move(-offset);
    }


//-----------------------------------------------------------------------------
void Sprite::setPosition (const Vector& pos)
    {
    pos_ = pos;   //syncing
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


//{LayerSprite::---------------------------------------------------------------
LayerSprite::LayerSprite (std::string name,
                          const sf::Texture& texture,
                          Vector pos,
                          sf::RenderWindow* windowp) :

    name_ (name),
    pos_(pos),
    windowp_(windowp)
    {
    layers_.push_back(Sprite ("L0",          //def layer
                              texture,
                              Vector (0, 0), //local cords/ or offset
                              windowp));
    }



//=============================================================================
void LayerSprite::draw ()
    {
    int textureExists = 1;
    for (auto& layer : layers_)
        textureExists *= (layer.getTexture()->getSize() != sf::Vector2u (0, 0));


    if (!textureExists)
        {
        deleteLayers();
        setTexture  (*AL::Global::DefaultSprite.getTexture());
        addLayer(Vector (0, 0), AL::Global::DefaultSprite.getAnimation(0));
        getLayer(0)->setPosition(pos_);
        }
    for (auto& layer : layers_)
            layer.draw(pos_);  //offset coords/ to draw in global, not in local coords
    }




//-----------------------------------------------------------------------------
void LayerSprite::deleteLayers ()
    {
    layers_.clear();
    }




//-----------------------------------------------------------------------------
void LayerSprite::addLayer (Vector localPos, AL::Animation animation)  //offset in fact
    {
    AL::Sprite newLayer(std::string ("L") + std::to_string(layers_.size()),
                        getTexture()? *getTexture() : sf::Texture(),
                        localPos,
                        windowp_);
    newLayer.addAnimation (animation);
    layers_.push_back(newLayer);
    }


//-----------------------------------------------------------------------------
void LayerSprite::setPosition (const Vector& pos)
    {
    pos_ = pos;
    }


//-----------------------------------------------------------------------------
void LayerSprite::setTexture (const sf::Texture& texture)
    {
    assert (!layers_.size());  //idk the solution^ letitbe this way/// so if u set the texture while having some layers drawing will broke
    for (auto& layer : layers_)
        layer.setTexture (texture);
    }


//-----------------------------------------------------------------------------
void LayerSprite::setRenderWindow (sf::RenderWindow* windowp)
    {
    windowp_ = windowp;
    }




//-----------------------------------------------------------------------------
const sf::Texture* LayerSprite::getTexture ()
    {
    return (layers_.size())? layers_.at(rand() % layers_.size()).getTexture() : nullptr;  //maid for fun/ as each layer has the same texture by design
    }


//-----------------------------------------------------------------------------
Vector LayerSprite::getPosition ()
    {
    return pos_;
    }


//-----------------------------------------------------------------------------
sf::RenderWindow* LayerSprite::getWindowp ()
    {
    return windowp_;
    }


//-----------------------------------------------------------------------------
AL::Sprite* LayerSprite::getLayer (int Id)
    {
    return &layers_.at(Id);
    }


//}
//-----------------------------------------------------------------------------



//}
//-----------------------------------------------------------------------------
}

//{Functions-------------------------------------------------------------------

void ExampleTest ()
    {
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

void nExampleTest ()
    {
    sf::RenderWindow win (sf::VideoMode (1000, 800), "test" );
    AL::Global::RenderWindow = &win;
    sf::Texture t;
    t.loadFromFile ("exsssample.jpg");

    AL::LayerSprite s ("imya", t, Vector (400, 400), &win);
    s.getLayer(0)->addAnimation(iVector (128, 128), iVector (8, 8), iVector (0, 0));

    while(!GetAsyncKeyState(VK_SPACE))
        {
        win.clear();
        s.draw();

        win.display();
        Sleep(20);
        }

    }

//-----------------------------------------------------------------------------

int UserMain();
int main()
    {
    sf::Texture defTexture;
    bool    load_succeeded = defTexture.loadFromFile("defTexture.jpg");
    assert (load_succeeded);

    AL::Global::DefaultSprite.setTexture (defTexture);
    AL::Global::DefaultSprite.setRenderWindow(AL::Global::RenderWindow);
    AL::Global::DefaultSprite.addAnimation (iVector (defTexture.getSize().x/2, defTexture.getSize().y),
                                            iVector (2, 1),
                                            iVector (0, 0) );
    #ifdef DEBUG
    nExampleTest();
    #else
    UserMain();
    #endif

    }
#define main UserMain


//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//TODO  многоанимационный спрайт, то бишь массив спрайтов => слои, добавить слой()
        //добавить анимацию на слой()
        //update()/пересчитывать относительные координаты спрайтов/




