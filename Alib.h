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

    virtual void setPosition     (const Vector& pos)          = 0;
    virtual void setTexture      (const sf::Texture* texture) = 0;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  = 0;

    virtual sf::RenderWindow*  getWindowp     ()              = 0;
    virtual const sf::Texture* getTexture     () const        = 0;
    virtual Vector             getPosition    ()              = 0;
    };


class      Sprite : public ISprite
    {
    std::string name_;
    sf::Sprite sprite_;
    Vector pos_;
    sf::RenderWindow*  windowp_;
    const sf::Texture* texture_;

    bool status_;

    std::vector <Animation> animations_;
    int animationId_;

    //---------------------
    public:
             Sprite (const Sprite& sprite);
    explicit Sprite (std::string name);
             Sprite (std::string name,
                     sf::RenderWindow* windowp,
                     const sf::Texture* texture,
                     Vector pos);

    virtual void draw () override;
            void draw (Vector offset);

    virtual void setPosition     (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture* texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;
            void setEnable (bool status);
            void setAnimationId  (int animationId);


            void addAnimation (Animation animation);
            void addAnimation (iVector frameSize,
                               iVector nFrames,
                               iVector pixFrameOffset);


    virtual sf::RenderWindow*  getWindowp     ()        override;
    virtual const sf::Texture* getTexture     () const  override;
    virtual Vector             getPosition    ()        override;
            int                getAnimationId ();
            Animation          getAnimation   (int animationId);


    Sprite& operator = (const Sprite& that) = delete;
    };


class LayerSprite : public ISprite
    {
    std::string name_;
    Vector pos_;
    sf::RenderWindow* windowp_;
    const sf::Texture* texture_;

    std::vector <Sprite> layers_;
    //-------------------
    public:
             LayerSprite (const LayerSprite& sprite);
    explicit LayerSprite (std::string name);
             LayerSprite (std::string name,
                          sf::RenderWindow* windowp,
                          const sf::Texture* texture,
                          Vector pos);

    virtual void draw () override;

    virtual void setPosition     (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture* texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;

            void addLayer (Vector localPos);
            void addLayer (Vector LocalPos, Animation animation);
            void enableLayer  (int ID, bool status);
            void enableLayers         (bool status);

    virtual sf::RenderWindow*  getWindowp     ()              override;
    virtual const sf::Texture* getTexture     ()  const       override;
    virtual Vector             getPosition    ()              override;

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
    return sf::IntRect (currentFrame_.x * pixFrameSize_.x + pixFrameOffset_.x,
                        currentFrame_.y * pixFrameSize_.y + pixFrameOffset_.y,
                                          pixFrameSize_.x,
                                          pixFrameSize_.y);
    }
//}
//-----------------------------------------------------------------------------


//{Sprite::--------------------------------------------------------------------
Sprite::Sprite (std::string name,
                sf::RenderWindow* windowp,
                const sf::Texture* texture,
                Vector pos) :

    name_ (name),
    sprite_ (),  //empty
    pos_ (pos),
    windowp_ (windowp),
    texture_ (texture),
    status_ (true),
    animations_ (),
    animationId_ (0)
    {
    assert (windowp);
    sprite_.setPosition(pos);

    if (texture && texture->getSize().x && texture->getSize().y)
        {
        sprite_.setTexture (*texture);  //user's texture
        }

    else
        {
        addAnimation (AL::Global::DefaultSprite.getAnimation(0));
        setTexture  (AL::Global::DefaultSprite.getTexture());    //default one
        }
    }


//-----------------------------------------------------------------------------
Sprite::Sprite (std::string name) :
    name_ (name),
    sprite_ (),
    pos_ (0, 0),
    windowp_ (Global::RenderWindow),
    texture_ (),
    status_ (true),
    animations_ (),
    animationId_ (0)
    {}
                                                                                                   //  i am coding all the day
//-----------------------------------------------------------------------------                    //  save me plz from feeling pai
Sprite::Sprite (const Sprite& sprite) :                                                            //  n
    name_        (sprite.name_),
    sprite_      (sprite.sprite_),
    pos_         (sprite.pos_),
    windowp_     (sprite.windowp_),
    texture_     (sprite.texture_),
    status_      (sprite.status_),
    animations_  (sprite.animations_),
    animationId_ (sprite.animationId_)
    {}

//=============================================================================
void Sprite::draw ()
    {
    assert(texture_);
    if (!status_) return;

    if (animations_.size())
        {
        Animation& animation = animations_.at(animationId_);

        sprite_.setTextureRect (animation.getCurrentFrame ());
        animation.update();                                                 //built-in update
        }
    else
        {
        sprite_.setTextureRect ( AllTexture(*sprite_.getTexture()) );
        }

    windowp_->draw(sprite_);
    }


//-----------------------------------------------------------------------------
void Sprite::draw (Vector offset)
    {
    if (!status_) return;

    sprite_.move(offset);
    draw ();
    sprite_.move(-offset);
    }




//-----------------------------------------------------------------------------
void Sprite::setEnable (bool status)
    {
    status_ = status;
    }


//-----------------------------------------------------------------------------
void Sprite::setPosition (const Vector& pos)
    {
    pos_ = pos;   //syncing
    sprite_.setPosition (pos_);
    }


//-----------------------------------------------------------------------------
void Sprite::setTexture (const sf::Texture* texture)
    {
    $sg; printf ("texture %d \n", texture != nullptr);


    if (texture && texture->getSize().x && texture->getSize().y)
        {
        sprite_.setTexture (*texture);  //user's texture
        }

    else
        {
        addAnimation (AL::Global::DefaultSprite.getAnimation(0));
        setAnimationId (int (animations_.size() - 1));
        setTexture  (AL::Global::DefaultSprite.getTexture());    //default one
        }
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
const sf::Texture* Sprite::getTexture () const
    {
    return texture_;
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
                          sf::RenderWindow* windowp,
                          const sf::Texture* texture,
                          Vector pos) :

    name_ (name),
    pos_(pos),
    windowp_(windowp),
    texture_(texture)
    {
    assert(windowp);

    if (texture)
        addLayer (Vector (0, 0));
    else
        {
        addLayer(Vector (0, 0), AL::Global::DefaultSprite.getAnimation(0));
        setTexture  (AL::Global::DefaultSprite.getTexture());
        }
    }



//=============================================================================
void LayerSprite::draw ()
    {

    for (auto& layer : layers_)
        layer.draw(pos_);  //offset coords/ to draw in global, not in local coords
    }




//-----------------------------------------------------------------------------
void LayerSprite::addLayer (Vector localPos, AL::Animation animation /*, const sf::Texture texture*/)  //offset in fact
    {
    AL::Sprite newLayer(std::string ("L") + std::to_string(layers_.size()),
                        windowp_,
                        getTexture(),
                        localPos);
    newLayer.addAnimation (animation);
    layers_.push_back(newLayer);
    }


//-----------------------------------------------------------------------------
void LayerSprite::addLayer (Vector localPos)
    {
    layers_.push_back ( AL::Sprite (std::string ("L") + std::to_string(layers_.size()),
                                   windowp_,
                                   getTexture(),
                                   localPos)    );
    }


//-----------------------------------------------------------------------------
void LayerSprite::enableLayer (int ID, bool status)
    {
    layers_.at(ID).setEnable(status);
    }


//-----------------------------------------------------------------------------
void LayerSprite::enableLayers (bool status)
    {
    for (auto& layer : layers_)
        layer.setEnable(status);
    }




//-----------------------------------------------------------------------------
void LayerSprite::setPosition (const Vector& pos)
    {
    pos_ = pos;
    }


//-----------------------------------------------------------------------------
void LayerSprite::setTexture (const sf::Texture* texture)
    {
    $sg; printf ("texture %d\n", texture != nullptr);

    if (texture)
        for (auto& layer : layers_)
            layer.setTexture (texture);
    else
        {
        enableLayers (false);
        addLayer (Vector (0, 0), AL::Global::DefaultSprite.getAnimation(0));
        setTexture  (AL::Global::DefaultSprite.getTexture());
        }
    }


//-----------------------------------------------------------------------------
void LayerSprite::setRenderWindow (sf::RenderWindow* windowp)
    {
    windowp_ = windowp;
    }




//-----------------------------------------------------------------------------
const sf::Texture* LayerSprite::getTexture () const
    {
    return texture_;
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
    t.loadFromFile ("res/example.jpg");

    AL::Sprite s ("imya", &win, &t, Vector (400, 400));
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
    t.loadFromFile ("res/ez.png");


    AL::LayerSprite s ("imya", &win, &t, Vector (400, 400));
    s.getLayer(0)->addAnimation(iVector (128, 128), iVector (8, 1), iVector (0, 0));
    //s.addLayer (Vector (-550, 0), AL::Animation (iVector (128, 128), iVector (7, 1), iVector (0, 0)) );

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
    bool    load_succeeded = defTexture.loadFromFile("res/defTexture.jpg");
    assert (load_succeeded);

    AL::Global::DefaultSprite.setTexture (&defTexture);
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



            //TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO
            //TODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODOTODO make draw() const, remoove the texture magic into constructors
                                                                                                                                                              //and setters
//TODO прийти к одному типу: у меня, шо косаеццо онемацций, когда добавляеццо онемацция и вибираеццо, собсна; а у слоев, все просто удаляеццо, и на прахе евонноном создаеццо кот
//к слову, сделать функцию able/disable слой в принципе вещь нужная


//gnu specific:::
// for (..;..;..)
// {
// if (!fn()) break;
// fn() || ({break; 0;});
// fn() || BREAK;  <-- define

//         делаем брейк выражением, чтобы его можно было подставлять в лог выражения
// }          так то это оператор/неззя/







