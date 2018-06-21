 //v1
#include "TXLib.h"

//TODO    local cords according to upper/parent widgetWidget (const std::string& name,

//DONE: wrapped sfEvents: closed, resized and mouse work,
//clickable and releasable widgets especially buttons,


#include <windows.h>
#include <assert.h>
#include "ALib.h"

//origin (pos_) is left top corner

//{Defines---------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------

//{Prototypes------------------------------------------------------------------
typedef sf::Vector2f     Vector;

namespace Global                                                                                            //
    {                                                                                                       //
    int Count = 0;                                                                                          //
    Vector MouseCoordinateDeformationMultiplier (1, 1);                                                     //
    const Vector OriginalWinSize (1000.0, 800.0);                                                           //
    const Vector ScreenSize (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));               //
    }

struct GroupWidget;


struct  Widget
    {
    std::string  name_; //for stuff only
    AL::Sprite   sprite_;
    Vector       pos_;
    Vector       size_;
    GroupWidget* parentWidget_;

    //-------------------------
    Widget (const std::string& name,
            const AL::Sprite&  sprite,
            const Vector&      pos          = Vector (AL::Global::FLAGVAL, AL::Global::FLAGVAL),
            const Vector&      size         = Vector (AL::Global::FLAGVAL, AL::Global::FLAGVAL));

    virtual void Click   (const Vector&   clickPos) = 0;
    virtual void Release (const Vector& releasePos) = 0;
    virtual void Resize  (const Vector& newSize );

    virtual void draw ();

    virtual bool mouseOnTheWidget (const Vector& mousePos);

            void setParentWidget (GroupWidget* parentWidget);



    };

struct        GroupWidget : public Widget
    {
    std::vector <Widget*> widgets_;
    //-----------------------------
    GroupWidget (const std::string& name   = "GW",
                 const Vector&      pos    = Vector (0, 0),
                 const Vector&      size   = Global::OriginalWinSize,
                       AL::Sprite   sprite = AL::Sprite ("GWS"));    //not const & because need to do def value in constructor);

    virtual void Click   (const Vector&   clickPos) override;
    virtual void Release (const Vector& releasePos) override;
    virtual void Resize  (const Vector& newSize)    override;

    virtual void draw ()                            override;

            void add (Widget* widget);


    };

struct        Button      : public Widget
    {
    enum STATUS {RELEASED, PRESSED};
         STATUS status_;

    //-------------------------

    Button (const std::string& name,
            const AL::Sprite&  sprite,
            const Vector& pos,
            const Vector& size);


    virtual void update () = 0;
    };

class               SingleButton : public Button
    {

    //--------------------------
    public:

    SingleButton (const std::string& name,
                  const AL::Sprite&  sprite,
                  const Vector&      pos,
                  const Vector&      size = Vector ( AL::Global::FLAGVAL,  AL::Global::FLAGVAL));
    private:


    virtual void Click   (const Vector&   clickPos) override;
    virtual void Release (const Vector& releasePos) override;

    virtual void update () override;



    };

class               SwitchButton : public Button
    {

    //--------------------------
    public:
    SwitchButton (const std::string& name,
                  const AL::Sprite&  sprite,
                  const Vector&      pos,
                  const Vector&      size = Vector ( AL::Global::FLAGVAL, AL::Global::FLAGVAL));
    private:

    virtual void Click   (const Vector&   clickPos) override;
    virtual void Release (const Vector& releasePos) override;


    virtual void update () override;
    };




struct EventMgr
    {
    sf::RenderWindow* renderWindow_;
         GroupWidget* toplvlWidget_;

    //-----------------------------

    EventMgr (GroupWidget*      toplvlWidget,
              sf::RenderWindow* renderWindow);

    bool run ();
    bool eventModule();
    void  drawModule();

    sf::RenderWindow* getWindow();
    };


struct AppWindow
    {
    sf::RenderWindow* renderWindow_;
    GroupWidget mainWidget_;
    EventMgr eventMgr_;
    //------------------
    AppWindow (sf::RenderWindow* renderWindow);
    bool run ();
    void add (Widget* widget);
    };

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


enum EVENT {NON = 0, START};

const sf::IntRect AllTexture (const sf::Texture &tex);
Vector operator / (const Vector& lvalue, int rvalue);

        const sf::IntRect AllTexture  (const sf::Texture &tex);                                             //
        const sf::IntRect The_Half_TextureRect (const sf::Texture &tex);                                    //
        void DrawMasksForButtons (GroupWidget* window);                                                     //
                                                                                                            //


//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------


//{AppWindow::-----------------------------------------------------------------
AppWindow::AppWindow (sf::RenderWindow* renderWindow) :
    renderWindow_ (renderWindow),
    mainWidget_ (),
    eventMgr_ (&mainWidget_, renderWindow)
    {
    AL::Global::RenderWindow = renderWindow; //important for Alib.h
    }

//=============================================================================

void AppWindow::add (Widget* widget)
    {
    mainWidget_.add (widget);
    }

//-----------------------------------------------------------------------------
bool AppWindow::run ()
    {
    return eventMgr_.run();
    }


//}
//-----------------------------------------------------------------------------



//{EventMgr::------------------------------------------------------------------


EventMgr::EventMgr (GroupWidget*      toplvlWidget,
                    sf::RenderWindow* renderWindow) :
    renderWindow_ (renderWindow),
    toplvlWidget_ (toplvlWidget)
    {}

//=============================================================================
bool EventMgr::run ()
    {
            drawModule();
    return eventModule();
    }



//-----------------------------------------------------------------------------
bool EventMgr::eventModule ()
    {
                                                                                                                                                                         //      ^ ^
    sf::Event event;                                                                                                                                                     //     ['^')
                                                                                                                                                                         //  (\/^ \|)
    while (renderWindow_->pollEvent (event))                                                                                                                              //    '  '
        {
        printf ("\nevent <%d> ", event.type);

        switch (event.type)
            {
            case sf::Event::Closed :
                {
                printf ("closing window..    ");

                return false;
                }

            case sf::Event::MouseButtonPressed :
                {
                iVector clickPos_pix (event.mouseButton.x, event.mouseButton.y);
                 Vector clickPos_crd (renderWindow_->mapPixelToCoords (clickPos_pix));

                printf (" serving click..     [%.0f, %.0f]    ", clickPos_crd.x, clickPos_crd.y);
                if (toplvlWidget_->mouseOnTheWidget(clickPos_crd)) {{$sg; printf ("+%s  ", toplvlWidget_->name_.c_str());} toplvlWidget_->Click (clickPos_crd);}
                                                               else {$sr; printf ("-%s  ", toplvlWidget_->name_.c_str());}

                return true;
                }

            case sf::Event::MouseButtonReleased :
                {
                iVector releasePos_pix (event.mouseButton.x, event.mouseButton.y);
                 Vector releasePos_crd (renderWindow_->mapPixelToCoords (releasePos_pix));

                printf ("serving releasing.. [%.0f, %.0f]    ", releasePos_crd.x, releasePos_crd.y);

                if (toplvlWidget_->mouseOnTheWidget(releasePos_crd)) {{$sg; printf ("+%s  ", toplvlWidget_->name_.c_str());} toplvlWidget_->Release (releasePos_crd);}
                                                                 else {$sr; printf ("-%s  ", toplvlWidget_->name_.c_str());}

                return true;
                }

            case sf::Event::Resized :
                {
                Vector newSize (event.size.width, event.size.height);

                printf ("serving resizing..");

                toplvlWidget_->Resize (newSize);


                return true;
                }

            default :
                {
                return true;
                }
            }
        }
        return true;
    }



//-----------------------------------------------------------------------------
void EventMgr::drawModule ()
    {
    renderWindow_->display ();
    renderWindow_->clear ();

    toplvlWidget_->draw();

    //for objs?

    }



//-----------------------------------------------------------------------------
sf::RenderWindow* EventMgr::getWindow()                              //надо ли?
    {
    return renderWindow_;
    }

//}
//-----------------------------------------------------------------------------




//{Widget::--------------------------------------------------------------------

Widget::Widget (const std::string& name,
                const AL::Sprite&  sprite,
                const Vector&      pos,
                const Vector&      size) :

    name_   (name),
    sprite_ (sprite),
    pos_    (pos),
    size_   (size),
    parentWidget_ (nullptr)
    {
    if (size_ == Vector (AL::Global::FLAGVAL, AL::Global::FLAGVAL))
        size_ = (Vector) sprite_.getTexture()->getSize();

    assert (size_.x > 0 && size_.y > 0);   //если размер виджета не пришел ни из параметров конструктора, ни из размера текстуры, то земля тебе пухом братишка
    }

//=============================================================================

void Widget::setParentWidget(GroupWidget* parentWidget)
    {
    parentWidget_ = parentWidget;
    }


//-----------------------------------------------------------------------------
void Widget::Resize  (const Vector& newSize)
    {
    //help yourself, users
    //TODO some behaviors to window resizing, mean unproportional sprite scaling
    }



//-----------------------------------------------------------------------------
bool Widget::mouseOnTheWidget (const Vector& mousePos)
    {
    return (pos_.x < mousePos.x && mousePos.x < pos_.x + size_.x &&
            pos_.y < mousePos.y && mousePos.y < pos_.y + size_.y);
    }




//-----------------------------------------------------------------------------
void Widget::draw ()
    {
    sprite_ .setPosition (pos_);
    sprite_ .draw ();
    }



//}
//-----------------------------------------------------------------------------



//{--------GroupWidget::-------------------------------------------------------
GroupWidget::GroupWidget (const std::string& name,
                          const Vector&      pos,
                          const Vector&      size,
                                AL::Sprite   sprite) :

    Widget (name, sprite, pos, size)
    {}


//=============================================================================

void GroupWidget::Click  (const Vector& clickPos)
    {

    for (auto& widget : widgets_)
        if (widget->mouseOnTheWidget(clickPos)) {{$sg; printf ("+%s  ", widget->name_.c_str());}    widget->Click (clickPos);}
                                            else {$sr; printf ("-%s  ", widget->name_.c_str());}
    }


//-----------------------------------------------------------------------------
void GroupWidget::Release(const Vector& releasePos)
    {

    for (auto& widget : widgets_)
        if (widget->mouseOnTheWidget(releasePos)) {{$sg; printf ("+%s  ", widget->name_.c_str());}  widget->Release (releasePos);}
                                              else {$sr; printf ("-%s  ", widget->name_.c_str());}
                                                                                                    //TODO only for buttons (dynamic_cast or var with identification of the class /defines/)
    }


//-----------------------------------------------------------------------------
void GroupWidget::Resize (const Vector& newSize)
    {
    Widget::Resize (newSize);
    for (auto& widget : widgets_)
        widget->Resize (newSize);
    }



//-----------------------------------------------------------------------------
void GroupWidget::add (Widget* widget)
    {
    widget->setParentWidget (this);
    widgets_.push_back(widget);
    }



//-----------------------------------------------------------------------------
void GroupWidget::draw ()
    {
    sprite_.setPosition (pos_);

    if (sprite_.getTexture()) sprite_.draw ();
    for (auto& widget : widgets_)
        {
        widget->draw();
        }
    }


//}
//-----------------------------------------------------------------------------



//{--------Button::------------------------------------------------------------

Button::Button (const std::string& name,
                const AL::Sprite&  sprite,
                const Vector& pos,
                const Vector& size) :

    Widget (name, sprite, pos, size),
    status_       (RELEASED)

    {}

//=============================================================================

//}
//-----------------------------------------------------------------------------

//{----------------SingleButton::----------------------------------------------

SingleButton::SingleButton (const std::string& name,
                            const AL::Sprite&  sprite,
                            const Vector& pos,
                            const Vector& size) :

    Button (name, sprite, pos, size)
    {}

//=============================================================================
void SingleButton::update ()
    {
    //...
    }




//-----------------------------------------------------------------------------
void SingleButton::Click (const Vector& clickPos)
    {
    status_ = PRESSED;
    printf ("\n   \"%s\" pressed\n", name_.c_str());
    }



//-----------------------------------------------------------------------------
void SingleButton::Release (const Vector& releasePos)
    {
    status_ = RELEASED;
    printf ("\n   \"%s\" released\n", name_.c_str());
    }

//}
//-----------------------------------------------------------------------------

//{----------------SwitchButton::----------------------------------------------

SwitchButton::SwitchButton (const std::string& name,
                            const AL::Sprite&  sprite,
                            const Vector& pos,
                            const Vector& size) :

    Button (name, sprite, pos, size)
    {}

//=============================================================================
void SwitchButton::update ()
    {
    }




//-----------------------------------------------------------------------------
void SwitchButton::Click (const Vector& clickPos)
    {
    status_ = (status_ == PRESSED)? RELEASED : PRESSED ;
    printf ("\n   \"%s\" %s\n", name_.c_str(), status_? "pressed" : "released");        //need aletlbit debug
    }



//-----------------------------------------------------------------------------
void SwitchButton::Release (const Vector& releasePos)
    {
    }

//}
//-----------------------------------------------------------------------------







//}
//-----------------------------------------------------------------------------

//{Functions-------------------------------------------------------------------


int main ()
    {
    sf::RenderWindow renderWindow (sf::VideoMode (Global::OriginalWinSize.x, Global::OriginalWinSize.y), "win");
    AppWindow win (&renderWindow);

            sf::Texture texture;
            texture.loadFromFile("res/byablo4ko.bmp");

            sf::Texture tex;
            tex.loadFromFile("res/example.jpg");


            AL::Sprite s1 ("main::s1", &renderWindow, &texture, Vector (0, 0));

            AL::Sprite s2 ("main::s2", &renderWindow, &tex, Vector (0, 0));
            s2.addAnimation(iVector (128, 128), iVector (8, 8), iVector (0, 0));


            SwitchButton butt ("Button",
                                s1,
                                Vector (400, 400));

            printf("meow");

            SwitchButton but  ("Button2",
                                s2,
                                Vector (600, 600));

            win.add (&butt);
            win.add (&but);

        //burzshua        //po-prolitarski
    while (win.run() && !GetAsyncKeyState (VK_SPACE))
        {
        Sleep (50);
        }

    }

/*sf::RenderWindow renderWindow (sf::VideoMode (Global::OriginalWinSize.x, Global::OriginalWinSize.y), "window");


    AL::Global::RenderWindow = &renderWindow; //important for Alib.h

            sf::Texture texture;
            texture.loadFromFile("byablo4ko.bmp");

            sf::Texture tex;
            tex.loadFromFile("example.jpg");

    GroupWidget top;
    EventMgr eveMgr (&top, &renderWindow);

            AL::Sprite s2 ("main::s2", tex, Vector (0, 0), &renderWindow);
            s2.addAnimation(iVector (128, 128), iVector (8, 8), iVector (0, 0));

            SwitchButton butt ("Button",
                                AL::Sprite ("main::s", texture, Vector (0, 0), &renderWindow),
                                Vector (400, 400));
            SwitchButton but  ("Button2",
                                s2,
                                Vector (600, 600));

            top.add (&butt);
            top.add (&but);

    while (eveMgr.run())
        {
        Sleep (50);
        }
         */
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


//TODOTODOTODO Engine {engmgr, toplvl, renderwindow}

//TODO    draggable : widget; beginDrag endDrag; while on drag probably draw widget sprite in moue pos; (like in wind)


