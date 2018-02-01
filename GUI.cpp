//v1
#include "TXLib.h"
//create WidgetMgr || SmartWidget : Widget witch will include array of  WidgetMgrs || SmartWidgets  for logic tree
// to extract the work with sf events into new class from the window witch must work with window parametrs only
//the extract class will send vector of click/release coords to top widgetMgr \
                         send bool of window status to the window class\
                        so the purpose is division
// + last def prmtr in widget - WidgetMgr* || SmartWidget* parentwindow


#include <windows.h>
#include <assert.h>
#include "ALib.h"

//origin (pos_) is left top corner

//{Defines---------------------------------------------------------------------

#define WINDOW_ADDTYPEBUTTON__DEFINITION( type )                             \
                                                                             \
void Window::add##type##Button (const std::string& name,                     \
                                      AL::Sprite&  sprite,                   \
                                const Vector&      size)                     \
    {                                                                        \
    widgets_.push_back (new type##Button(name, sprite, *this,                \
                                            sprite.getPosition(),  size ));  \
    }


#define WINDOW_ADDTYPEBUTTON__PROTOTYPE( type )                              \
                                                                             \
void add##type##Button (const std::string& name,                             \
                              AL::Sprite&  sprite,                           \
                        const Vector&      size);                            \
//}
//-----------------------------------------------------------------------------

//{Prototypes------------------------------------------------------------------

typedef sf::Vector2f     Vector;
            struct Window;



struct  Widget
    {
    std::string name_; //for stuff only
    AL::Sprite  sprite_;
    Window&     parentWindow_;

    Vector      pos_;
    Vector      size_;

    //-------------------------
    Widget (const std::string& name,
                  AL::Sprite&  sprite,
                  Window& parentWindow,
            const Vector& pos,
            const Vector& size);

    virtual void serveClick (const Vector& ClickPos);
     virtual void serveRelease () = 0;       //TODO /need optimization/ widget logically doesnt need such fn/ the idea is to keep objects in Window in many ars: widgets, buttons..
    virtual void onClick ()    = 0;    //   O   I
  //virtual void onMouse ()    = 0;    // L   G   C


    virtual void draw ();
            void setSpriteAnimation (int animation);
            void addSpriteAnimation (iVector animationBeginingPos,
                                     iVector frameSize,
                                     iVector nFrames);


    virtual bool mouseOnTheWidget (const Vector& mousePos);

    };


struct        Button : public Widget
    {
    enum STATUS {RELEASED, PRESSED};
         STATUS status_;

    //-------------------------

    Button (const std::string& name,
                  AL::Sprite&  sprite,
                  Window& parentWindow,
            const Vector& pos,
            const Vector& size);

    virtual void serveClick (const Vector& ClickPos) = 0;
    //virtual void serveRelease ()                     = 0;      because a temporary put it in Widget

    virtual void onClick ()                          = 0;

    virtual void update ()                           = 0;
    };

class               SingleButton : public Button
    {

    //--------------------------
    public:
    SingleButton (const std::string& name,
                       AL::Sprite&  sprite,
                       Window& parentWindow,
                  const Vector& pos,
                  const Vector& size = Vector (0, 0));
    private:

    virtual void serveClick   (const Vector& ClickPos) override;
    virtual void serveRelease () override;

    virtual void onClick () override;

    virtual void update () override;
    };

class               SwitchButton : public Button
    {

    //--------------------------
    public:
    SwitchButton (const std::string& name,
                       AL::Sprite&  sprite,
                       Window& parentWindow,
                  const Vector& pos,
                  const Vector& size = Vector (0, 0));
    private:

    virtual void serveClick   (const Vector& ClickPos) override;
    virtual void serveRelease () override;

    virtual void onClick () override;

    virtual void update () override;
    };


struct Window
    {
    enum STATUS {CLOSED, OPENED};

    const std::string name_;
    sf::RenderWindow window_;
    std::vector<Widget*> widgets_;

    STATUS status_;

    //-----------------------------

    Window (const std::string& name, Vector size, Vector pos);
   ~Window ();

    bool run ();

    bool checkEvents ();
    void serveClick (Vector ClickPos);
    void serveRelease();

        void addNew (Widget* widget);
        WINDOW_ADDTYPEBUTTON__PROTOTYPE (Single)
        WINDOW_ADDTYPEBUTTON__PROTOTYPE (Switch)


        void draw ();
    sf::RenderWindow& getWindow();

    };


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


enum EVENT {NON = 0, START};

const sf::IntRect AllTexture (const sf::Texture &tex);
Vector operator / (const Vector& lvalue, int rvalue);

        const sf::IntRect AllTexture  (const sf::Texture &tex);                                             //
        const sf::IntRect The_Half_TextureRect (const sf::Texture &tex);                                    //
        void DrawMasksForButtons (Window* window);                                                          //
                                                                                                            //
namespace Global                                                                                            //
    {                                                                                                       //
    sf::RenderWindow* Window = nullptr;                                                                     //
    const Vector WinSize (200, 200);                                                                        //
    const Vector ScreenSize (GetSystemMetrics (SM_CXSCREEN), GetSystemMetrics (SM_CYSCREEN));               //
    }


//}
//-----------------------------------------------------------------------------


//=======================================================================================


//{Classes---------------------------------------------------------------------




//{Window::--------------------------------------------------------------------






Window::Window (const std::string& winName, Vector winSize, Vector winPos)  :
    name_ (winName),
    status_ (OPENED),
    //pos_
    window_ (sf::VideoMode (winSize.x, winSize.y), winName)
    {

    window_.setPosition((sf::Vector2i) (winPos + Global::ScreenSize/2 - winSize/2));

    int balance = 10 - name_.length ();
    printf ("   Win \"%s\" %*s is created and opened\n",    name_.c_str(), balance, "");
    }



//-----------------------------------------------------------------------------
Window::~Window()
    {
    assert (status_ == CLOSED);
    int balance = 10 - name_.length ();
    printf ("   Win \"%s\" %*s is closed and destructed\n", name_.c_str(), balance, "");
    }



//=============================================================================
bool Window::run ()
    {
    draw ();

    return checkEvents ();
    }



//-----------------------------------------------------------------------------
sf::RenderWindow& Window::getWindow()
    {
    return window_;
    }


//-----------------------------------------------------------------------------
void Window::addNew (Widget* widget)
    {
    widgets_.push_back (widget);
    }

WINDOW_ADDTYPEBUTTON__DEFINITION (Single)
WINDOW_ADDTYPEBUTTON__DEFINITION (Switch)



//-----------------------------------------------------------------------------
bool Window::checkEvents ()
    {

    sf::Event event;

    while (window_.pollEvent (event) )
        {
        printf ("\r   event <%d>\n", event.type);   // \r for redrawing console loading

        switch (event.type)
            {
            case sf::Event::Closed :
                {
                printf ("       closing window..\n");
                status_ = CLOSED;
                return false;
                }

            case sf::Event::MouseButtonPressed :
                {
                printf ("       serving click..\n");
                serveClick (Vector (event.mouseButton.x,
                                    event.mouseButton.y) );
                return true;
                }

            case sf::Event::MouseButtonReleased :
                {
                printf ("       serving releasing..\n");
                serveRelease ();
                return true;
                }

            default :
                {
                printf ("       ");
                return true;
                }
            }
        }
        return true;
    }



//-----------------------------------------------------------------------------
void Window::serveClick(Vector ClickPos)
    {
    printf ("       click (%g, %g)\n", ClickPos.x, ClickPos.y);

    for (auto& widget : widgets_)
        widget->serveClick(ClickPos);

    printf ("           ");
    }



//-----------------------------------------------------------------------------
void Window::serveRelease()
    {

    for (auto& widget : widgets_)
        widget->serveRelease();            //TODO only for buttons (dynamic_cast or var with identification of the class /defines/)

    printf ("           ");
    }



//-----------------------------------------------------------------------------
void Window::draw ()
    {
    window_.display ();
    window_.clear ();

    for (auto& widget : widgets_)
        {
        widget->draw();
        }
    //for objs?

    }


//}
//-----------------------------------------------------------------------------




//{Widget::--------------------------------------------------------------------

Widget::Widget (const std::string& name,
                AL::Sprite&  sprite,
                Window& parentWindow,
                const Vector& pos,
                const Vector& size) :

    name_ (name),
    sprite_   (sprite),
    parentWindow_ (parentWindow),
    pos_  (pos),
    size_ (size)
    {
    sprite.setPosition (pos);
    if (size_ == Vector (0, 0))
        size_ = (Vector) sprite_.getTextureSize();
    }

//=============================================================================

//-----------------------------------------------------------------------------
void Widget::serveClick (const Vector& clickPos)
    {
    if (mouseOnTheWidget (clickPos)) onClick();
    }



//----------------R-------W--------N-------------------------------------------
//------------D-------A-------I--------G---------------------------------------
void Widget::draw ()
    {
    sprite_ .draw ();
    }



//-----------------------------------------------------------------------------
void Widget::setSpriteAnimation (int animation)
    {
    sprite_ .setAnimation (animation);
    }



//-----------------------------------------------------------------------------
void Widget::addSpriteAnimation (iVector animationBeginingPos,
                                 iVector frameSize,
                                 iVector nFrames)
    {
    sprite_. addAnimation (animationBeginingPos, frameSize, nFrames);
    }



//-----------------------------------------------------------------------------
bool Widget::mouseOnTheWidget (const Vector& mousePos)
    {
    return (pos_.x < mousePos.x && mousePos.x < pos_.x + size_.x &&
            pos_.y < mousePos.y && mousePos.y < pos_.y + size_.y);
    }

//}
//-----------------------------------------------------------------------------


//{--------Button::------------------------------------------------------------

Button::Button (const std::string& name,
                      AL::Sprite&  sprite,
                      Window& parentWindow,
                const Vector& pos,
                const Vector& size) :

    Widget (name, sprite, parentWindow, pos, size),
    status_       (RELEASED)

    {}

//=============================================================================


void Button::update ()
    {}

//-----------------------------------------------------------------------------

void Button::onClick ()
    {}

//-----------------------------------------------------------------------------
void Button::serveClick (const Vector& ClickPos)
    {}

//-----------------------------------------------------------------------------
//void Button::serveRelease ()
 //   {}                            temporar


//}
//-----------------------------------------------------------------------------

//{----------------SingleButton::----------------------------------------------

SingleButton::SingleButton (const std::string& name,
                                  AL::Sprite&  sprite,
                                  Window& parentWindow,
                            const Vector& pos,
                            const Vector& size) :

    Button (name, sprite, parentWindow, pos, size)
    {}

//=============================================================================
void SingleButton::update ()
    {

    //sf::IntRect frame (sprite_.getTexture()->getSize().x/2 * (int)status_, 0, sprite_.getTexture()->getSize().x/2 , sprite_.getTexture()->getSize().y) ;

    //sprite_.setTextureRect (frame);

    }




//-----------------------------------------------------------------------------
void SingleButton::onClick ()
    {
    status_ = PRESSED;
    printf ("               \"%s\" new status = pressed\n", name_.c_str());
    }



//-----------------------------------------------------------------------------
void SingleButton::serveClick (const Vector& clickPos)
    {
    if (mouseOnTheWidget (clickPos))
        {
        printf ("           \"%s\" is onClick\n", name_.c_str());
        onClick();
        return;
        }
    else printf ("            \"%s\" Click ignored\n", name_.c_str());
    }



//-----------------------------------------------------------------------------
void SingleButton::serveRelease ()
    {
    status_ = RELEASED;
    printf ("           \"%s\" new status = released\n", name_.c_str());
    }

//}
//-----------------------------------------------------------------------------

//{----------------SwitchButton::----------------------------------------------

SwitchButton::SwitchButton (const std::string& name,
                                  AL::Sprite&  sprite,
                                  Window& parentWindow,
                            const Vector& pos,
                            const Vector& size) :

    Button (name, sprite, parentWindow, pos, size)
    {}

//=============================================================================
void SwitchButton::update ()
    {

    //sf::IntRect frame (sprite_.getTexture()->getSize().x/2 * (int)status_, 0, sprite_.getTexture()->getSize().x/2 , sprite_.getTexture()->getSize().y) ;

    //sprite_.setTextureRect (frame);

    }




//-----------------------------------------------------------------------------
void SwitchButton::onClick ()
    {
    status_ = (status_ == PRESSED)? RELEASED : PRESSED ;
    printf ("               \"%s\" new status = %s\n", name_.c_str(), status_? "pressed" : "released");        //need aletlbit debug
    }




//-----------------------------------------------------------------------------
void SwitchButton::serveClick (const Vector& clickPos)
    {

    if (mouseOnTheWidget (clickPos))
        {
        printf ("            \"%s\" is onClick\n",     name_.c_str());
        onClick();
        return;
        }
    else printf ("            \"%s\" Click ignored\n", name_.c_str());
    }



//-----------------------------------------------------------------------------
void SwitchButton::serveRelease ()
    {
    printf ("           \"%s\" releasing ignored\n", name_.c_str());
    }

//}
//-----------------------------------------------------------------------------







//}
//-----------------------------------------------------------------------------

//{Functions-------------------------------------------------------------------

int main ()
    {
    Window win ("test", Vector (1800, 1000), Vector (0, 0));

    sf::Texture texture;
    texture.loadFromFile("byablo4ko.bmp");
    AL::Sprite s (texture, Vector (500, 500), &win.getWindow());
    s.addAnimation ();
    s.setAnimation (0);
    win.addSwitchButton ("red", s, (Vector) s.getTextureSize());



    int t = 0;

    printf ("%10s", "");

    while (win.run())
        {
        printf ("\b%c", "-\\|/"[++t%4]);

        //s.draw();

        Sleep (100);
        }

    win.getWindow().close();
    }



//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



                     //     /        /
                     //   \/       \/
//TODO virtual buttons: switch, single, timerbutton, hotkeybutton...                             banner like stbst?




