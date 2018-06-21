//ANIMATION LIB v 1.1.4

#include <sfml/graphics.hpp>
#include "AlibOperators.hpp"
#include <windows.h>
#include  <vector>

//#define DEBUG

namespace AL
{/*Classes functions and prototypes*/
//{Definition------------------------------------------------------------------
namespace Global
{
const int FLAGVAL = -100500;
long long CPUFrequency = 0;
double    FPS = 25;
}


struct Animation
    {
    iVector currentFrame_;

    iVector pixFrameSize_;
    iVector nFrames_;
    iVector pixFrameOffset_;


    int ticksPerFrame_;

    private:

    long long lastFrameChangeTime_;

    public:

    //---------------------
    Animation (iVector pixFrameOffset,
               iVector pixFrameSize,
               iVector nFrames,
               double FPS = AL::Global::FPS);

    sf::IntRect getCurrentFrame () const;
    void update ();
    };


struct    ISprite
    {

    //---------------------
    virtual ~ISprite () {};

    virtual void draw () = 0;

    virtual void setPosition     (const Vector& pos)          = 0;
    virtual void move            (const Vector& pos)          = 0;
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
    virtual void move            (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture* texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;
            void setEnable       (bool status);             //TODO    virt?
            void setAnimationId  (int animationId);


            void addAnimation (Animation animation);
            void addAnimation (iVector frameSize,
                               iVector nFrames,
                               iVector pixFrameOffset,
                               double FPS = AL::Global::FPS);


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
    Vector globalPos_;
    sf::RenderWindow* windowp_;
    const sf::Texture* texture_;

    bool allTexturesValid_;

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
    virtual void move            (const Vector& pos)          override;
    virtual void setTexture      (const sf::Texture* texture) override;
    virtual void setRenderWindow (sf::RenderWindow* windowp)  override;

            void enableLayer  (int ID, bool status);
            void enableLayers         (bool status);
            void addErrLayer ();
            bool addLayer (Vector LocalPos,
                           AL::Animation animation     = AL::Animation (iVector (0, 0),
                                                                        iVector (0, 0),    //flag
                                                                        iVector (0, 0)),
                           const sf::Texture*  texture = nullptr);

    virtual sf::RenderWindow*  getWindowp     ()              override;
    virtual const sf::Texture* getTexture     ()  const       override;
    virtual Vector             getPosition    ()              override;

                   AL::Sprite*     getLayer (int Id);
             const AL::Sprite*     getLayer (int Id) const;
                   AL::Sprite&  operator [] (int Id);
    LayerSprite& operator = (const LayerSprite& that) = delete;
    };


namespace Global
{
Sprite DefaultSprite ("AJIJIO_ETO_TEXHUK_IIAshA?\nY HAC TYT TEXHUKA HE IIAshET!");
sf::RenderWindow *RenderWindow = nullptr;
}
//}
//-----------------------------------------------------------------------------


//{stuff_fns-------------------------------------------------------------------

void Draw (AL::Sprite&      sprite);
void Draw (AL::Sprite&      sprite, Vector pos);

void Draw (AL::LayerSprite& sprite);
void Draw (AL::LayerSprite& sprite, Vector pos);

bool operator == (const AL::Animation& left, const AL::Animation& right);
bool operator != (const AL::Animation& left, const AL::Animation& right);
void  ExampleTest ();
void nExampleTest ();

//}
//=============================================================================
//{external_fns----------------------------------------------------------------
void Draw (AL::Sprite& sprite)
    {
    sprite.draw();
    }


//-----------------------------------------------------------------------------
void Draw (AL::Sprite& sprite, Vector drawPos)
    {
    Vector pos = sprite.getPosition ();

    sprite.setPosition (drawPos);
    sprite.draw();

    sprite.setPosition (pos);
    }



//-----------------------------------------------------------------------------
void Draw (AL::LayerSprite& sprite)
    {
    sprite.draw();
    }


//-----------------------------------------------------------------------------
void Draw (AL::LayerSprite& sprite, Vector drawPos)
    {
    Vector pos = sprite.getPosition ();

    sprite.setPosition (drawPos);
    sprite.draw();

    sprite.setPosition (pos);
    }


//}
//-----------------------------------------------------------------------------



//{Classes---------------------------------------------------------------------



//{Animation::-----------------------------------------------------------------
Animation::Animation (iVector pixFrameSize,
                      iVector nFrames,
                      iVector pixFrameOffset,
                      double FPS) :
    currentFrame_ ( iVector (0, 0) ),      //1st frame 1st row /cap/

    pixFrameSize_   (pixFrameSize),
    nFrames_        (nFrames),
    pixFrameOffset_ (pixFrameOffset),

    ticksPerFrame_       (ROUND ( (double) AL::Global::CPUFrequency / FPS) ),
    lastFrameChangeTime_ (0)
    {}


//=============================================================================
void Animation::update ()
    {
    LARGE_INTEGER ticksCounter = {{0}};
    QueryPerformanceCounter (&ticksCounter);
    long long time = ticksCounter.QuadPart;

    if  (time - lastFrameChangeTime_ > ticksPerFrame_)  //FPS ctrl a lil bit
        {
        currentFrame_.x++;
                                           //плюс тот факт, что это так или не так
        currentFrame_.y = (currentFrame_.y + (currentFrame_.x >= nFrames_.x)) % nFrames_.y;
        currentFrame_.x =  currentFrame_.x % nFrames_.x;

        lastFrameChangeTime_ = time;
        }
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
    {
    }
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
    //printf ("   %s  ", name_.c_str());
    assert(texture_);
    assert(texture_->getSize().x && texture_->getSize().y);
    assert(windowp_);
    if (!status_) return;

    if (animations_.size())     {
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
void Sprite::move (const Vector& pos)
    {
    pos_ += pos;
    }


//-----------------------------------------------------------------------------
void Sprite::setTexture (const sf::Texture* texture)
    {
    $sg; printf ("texture existence = %d, size %s %s\n", texture != nullptr, (texture)? std::to_string (texture->getSize().x).c_str() : "???", (texture)? std::to_string (texture->getSize().y).c_str() : "???"); //Ones, one guy remooved printfs from his program, nobody'v seen him since then

    if (texture && texture->getSize().x && texture->getSize().y)
        {
        sprite_.setTexture (*texture);  //user's texture
        printf ("mew");
        texture_ = texture;
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
                           iVector pixFrameOffset,
                           double FPS)
    {
    animations_.push_back (Animation (pixFrameSize,
                                      nFrames,
                                      pixFrameOffset,
                                      FPS) );
    }




//-----------------------------------------------------------------------------
const sf::Texture* Sprite::getTexture () const
    {
    assert(texture_ == sprite_.getTexture());
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
                          Vector globalPos) :

    name_ (name),
    globalPos_(globalPos),
    windowp_(windowp),
    texture_(texture),
    allTexturesValid_ (true),
    layers_ ()
    {
    if (!windowp) assert(!"you can't miss renderwindow param");

    if (texture)        //background layer
        layers_.push_back( AL::Sprite (std::string ("L") + std::to_string(layers_.size()),
                                       windowp,
                                       texture,
                                       Vector (0, 0)) ); //local pos
    else
        {
        allTexturesValid_ = false;
        addErrLayer ();
        }
    }



//=============================================================================
void LayerSprite::draw ()
    {
    for (auto& layer : layers_)
        layer.draw(globalPos_);  //offset coords/ to draw in global, not in local coords
    }




//-----------------------------------------------------------------------------
void LayerSprite::addErrLayer ()
    {
    layers_.push_back ( AL::Sprite (std::string ("L") + std::to_string(layers_.size()),
                                    windowp_,
                                    nullptr,
                                    Vector (0, 0)) );
    }


//-----------------------------------------------------------------------------
bool LayerSprite::addLayer (Vector localPos, AL::Animation animation, const sf::Texture* texture)  //localPos is offset from background in fact
    {

    if (texture && !texture->getSize().x && !texture->getSize().y)
        {

        if (allTexturesValid_)
            {
            printf("fff");
            allTexturesValid_ = false;
            enableLayers (false);           //this sheet is for drawing one neat cat instead of all the crashed and valid layers
            addErrLayer  ();
            }
        else
            {

            addErrLayer ();
            getLayer( (int) layers_.size() - 1)->setEnable(false);
            }
        return false;
        }


    AL::Sprite newLayer(std::string ("L") + std::to_string(layers_.size()),
                        windowp_,
                        (texture)? texture : getTexture(), //if one does not set the last param, the texture will be taken from the background
                        localPos);
    if (animation != AL::Animation (iVector (0, 0), iVector (0, 0), iVector (0, 0)) ) newLayer.addAnimation (animation);
    if (!allTexturesValid_) newLayer.setEnable(false);
    layers_.push_back(newLayer);

    return true;
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
    globalPos_ = pos;
    }


//-----------------------------------------------------------------------------
void LayerSprite::move (const Vector& pos)
    {
    globalPos_ += pos;
    }


//-----------------------------------------------------------------------------
void LayerSprite::setTexture (const sf::Texture* texture)
    {
    $sg; printf ("texture %d\n", texture != nullptr);

    if (texture)
        {
        for (auto& layer : layers_)
            if (layer.getTexture() == getTexture()) layer.setTexture (texture);         //woooow
        }
    else
        {
        enableLayers (false);
        allTexturesValid_ = false;
        addErrLayer ();
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
    return getLayer(0)->getTexture();
    }


//-----------------------------------------------------------------------------
Vector LayerSprite::getPosition ()
    {
    return globalPos_;
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


//-----------------------------------------------------------------------------
const AL::Sprite* LayerSprite::getLayer (int Id) const
    {
    return &layers_.at(Id);
    }


//-----------------------------------------------------------------------------
AL::Sprite& LayerSprite::operator [] (int Id)
    {
    return layers_.at(Id);
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
    win.setFramerateLimit(25);

    AL::Global::RenderWindow = &win;
    sf::Texture t;
    sf::Texture ponytrash;
    sf::Texture te;
    ponytrash.loadFromFile ("res/ANIMEGIRL.png");
    t.loadFromFile ("res/ez.png");
    te.loadFromFile ("res/example.png");



    AL::LayerSprite s ("imya", &win, &t, Vector (400, 400));
    s[0].addAnimation(iVector (128, 128), iVector (8, 1), iVector (0, 0), 2);
    s.addLayer (Vector (32, 0), AL::Animation (iVector (128, 128), iVector (8, 8), iVector (0, 0), 9), &te );

    while(!GetAsyncKeyState(VK_SPACE))
        {

        Vector move (0, 0);

        if (GetAsyncKeyState('W')) move.y = -10;
        if (GetAsyncKeyState('A')) move.x = -10;
        if (GetAsyncKeyState('S')) move.y =  10;
        if (GetAsyncKeyState('D')) move.x =  10;


        s.move (move);

        win.clear();
        s.draw();

        win.display();
        //Sleep(20);
        }

    }

//-----------------------------------------------------------------------------

int UserMain();
int main()
    {
    LARGE_INTEGER freq = {{0}};
    QueryPerformanceFrequency (&freq);
    AL::Global::CPUFrequency = freq.QuadPart;

    sf::Texture defTexture;
    bool    load_succeeded = defTexture.loadFromFile("res/defTexture.jpg");
    assert (load_succeeded);

    AL::Global::DefaultSprite.setTexture (&defTexture);
    AL::Global::DefaultSprite.setRenderWindow(AL::Global::RenderWindow);
    AL::Global::DefaultSprite.addAnimation (iVector (defTexture.getSize().x/2, defTexture.getSize().y),
                                            iVector (2, 1),
                                            iVector (0, 0) );

    assert(AL::Global::DefaultSprite.getTexture());
    #ifdef DEBUG
    nExampleTest();
    #else
    UserMain();
    #endif

    }
#define main UserMain


//{operators-------------------------------------------------------------------

bool AL::operator == (const AL::Animation& left, const AL::Animation& right)
    {
    return (left.pixFrameSize_   == right.pixFrameSize_ &&
            left.nFrames_        == right.nFrames_      &&
            left.pixFrameOffset_ == right.pixFrameOffset_);
    }


//-----------------------------------------------------------------------------
bool AL::operator != (const AL::Animation& left, const AL::Animation& right)
    {
    return !(left == right);
    }


//}
//-----------------------------------------------------------------------------




//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


/*poor description:
sfml wrap
drawable,
animated,
layered objects

1.1.0   in LSprite first layer /created in constructor/ is background, LS::getTexture() and LS::setTexture() work with background texture
        you can create a layer with background texture, if not set the last param in LS::addLayer(...) /So you can create a layer with dif tex by setting the param/

1.1.1   operator [] for layers
1.1.2   fps control     +LSprite::setFPS
                        +Sprite::setFPS
                        +Animation::setFPS



1.1.3 remooved warnings :>

1.1.4 + 4 type of Draw(sprite)
*/

//other stuff
//gnu specific:::
// for (..;..;..)
// {
// if (!fn()) break;
// fn() || ({break; 0;});
// fn() || BREAK;  <-- define

//         делаем брейк выражением, чтобы его можно было подставлять в лог выражения
// }          так то это оператор/неззя/








