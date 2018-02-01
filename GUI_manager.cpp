//v0
/*#include "ALib.h" */ #include <sfml/graphics.hpp>
#include <windows.h>
#include <assert.h>

//TODO DONT RETURN WITHOUT AN ANIMATION LIB

//{Defines---------------------------------------------------------------------

#define WINDOW_ADDTYPEBUTTON__DEFINITION( type )                             \
                                                                             \
void Window::add##type##Button (const std::string& name,                     \
                                      sf::Sprite& sprite,                    \
                                      Window& parentWindow,                  \
                                const Vector& pos,                           \
                                const Vector& size = Vector (0, 0))          \
    {                                                                        \
    buttons_.push_back (new type##Button(name, sprite, *this, pos, size));   \
    }


#define WINDOW_ADDTYPEBUTTON__PROTOTYPE( type )                              \
                                                                             \
void add##type##Button (const std::string& name,                             \
                              sf::Sprite& sprite,                            \
                              Window& parentWindow,                          \
                        const Vector& pos,                                   \
                        const Vector& size);                                 \
//}
//-----------------------------------------------------------------------------

//{Prototypes------------------------------------------------------------------

typedef sf::Vector2f     Vector;
            struct Window;

struct                      Button
    {
    enum STATUS {RELEASED, PRESSED};


    std::string name_;

    sf::Sprite  sprite_;
    Window&     parentWindow_;

    Vector      pos_;
    Vector      size_;

    STATUS      status_;

    //-------------------------

    Button (const std::string& name,
                  sf::Sprite&  sprite,
                  Window& parentWindow,
            const Vector& pos,
            const Vector& size);

    virtual void serveClick (const Vector& ClickPos) = 0;
    virtual void serveRelease ()                     = 0;

    virtual void onClick ()                          = 0;

    virtual void update ()                           = 0;


        virtual bool mouseOnTheWidget (const Vector& mousePos);
    };

class SingleButton : public Button
    {

    //--------------------------
    public:
    SingleButton (const std::string& name,
                       sf::Sprite&  sprite,
                       Window& parentWindow,
                  const Vector& pos,
                  const Vector& size = Vector (0, 0));
    private:

    virtual void serveClick   (const Vector& ClickPos) override;
    virtual void serveRelease () override;

    virtual void onClick () override;

    virtual void update () override;
    };

class SwitchButton : public Button
    {

    //--------------------------
    public:
    SwitchButton (const std::string& name,
                       sf::Sprite&  sprite,
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
    std::vector<Button*> buttons_;

    STATUS status_;

    //-----------------------------

    Window (const std::string& name, Vector size, Vector pos);
   ~Window ();

    bool checkEvents ();
    void serveClick (Vector ClickPos);
    void serveRelease();

    void addNew (Button* button);
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

sf::RenderWindow& Window::getWindow()
    {
    return window_;
    }


//-----------------------------------------------------------------------------
void Window::addNew (Button* button)
    {
    buttons_.push_back (button);
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

    for (auto& button : buttons_)
        button->serveClick(ClickPos);

    printf ("           ");
    }



//-----------------------------------------------------------------------------
void Window::serveRelease()
    {

    for (auto& button : buttons_)
        button->serveRelease();

    printf ("           ");
    }



//-----------------------------------------------------------------------------
void Window::draw ()
    {
    window_.clear ();
    for (auto& button : buttons_)
        {
        //button->update();
        window_.draw(button->sprite_);
        if (GetAsyncKeyState (VK_SPACE)) DrawMasksForButtons (this);
        }
    window_.display();
    }


//}
//-----------------------------------------------------------------------------



//{Button::--------------------------------------------------futurewiget-------

Button::Button (const std::string& name,
                      sf::Sprite&  sprite,
                      Window& parentWindow,
                const Vector& pos,
                const Vector& size) :

    name_         (name),
    sprite_       (sprite),
    parentWindow_ (parentWindow),
    pos_          (pos),

    size_         (size),
    status_       (RELEASED)

    {
    sprite_.setPosition (pos_);

    if (size_ == Vector (0, 0))  size_ = (Vector) sprite_.getTexture()->getSize();
    }

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
void Button::serveRelease ()
    {}

//-----------------------------------------------------------------------------

bool Button::mouseOnTheWidget (const Vector& mousePos)
    {                                                                             //TODO widget
    return (pos_.x < mousePos.x && mousePos.x < pos_.x + size_.x &&
            pos_.y < mousePos.y && mousePos.y < pos_.y + size_.y);           //rect
    }                                                                                 //!! !

//}
//-----------------------------------------------------------------------------

//{--------SingleButton::------------------------------------------------------

SingleButton::SingleButton (const std::string& name,
                                  sf::Sprite&  sprite,
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

//{--------SwitchButton::------------------------------------------------------

SwitchButton::SwitchButton (const std::string& name,
                                  sf::Sprite&  sprite,
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
    {                                                                                   //TODO please notice me senpai (c) Button::pos_
    Window win ("test", Vector (1800, 1000), Vector (0, 0));

    sf::Texture texture;
    texture.loadFromFile("byablo4ko.bmp");
    sf::Sprite sprite (texture, AllTexture (texture));

    win.addSwitchButton ("red", sprite, win, Vector (300, 300));



    int t = 0;

    printf ("%10s", "");

    while (1)
        {
        printf ("\b%c", "-\\|/"[++t%4]);

        if (!win.checkEvents()) break;
        win.draw ();

        Sleep (100);
        }

    win.getWindow().close();
    }


//-----------------------------------------------------------------------------
void DrawMasksForButtons (Window* window)
    {
    for (auto& button : window->buttons_)
        {
        sf::RectangleShape mask (button->size_);
        mask.setPosition        (button->pos_);

        mask.setFillColor (sf::Color ((int) button->pos_.x  % 200,
                                      (int) button->pos_.y  % 200,
                                      (int) button->size_.x % 200));

        window->getWindow().draw (mask);
        }
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



                     //     /        /
                     //   \/       \/
//TODO virtual buttons: switch, single, timerbutton, hotkeybutton...                             banner like stbst?



//TODO last default parametr int constructor class button : clickarea


//TODO v size && v pos instead of clickarea



