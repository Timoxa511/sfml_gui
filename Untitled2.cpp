#define _USE_MATH_DEFINES

//#include <txLIb.h>
// #define _TX_NOINIT
#pragma GCC diagnostic ignored "-Weffc++"

#include <sfml/graphics.hpp>
#include <windows.h>
#include <math.h>
#include <assert.h>
#include <string>

//{predefinitions---------------------------------------------------------------
struct    Bullet;
//}
//-----------------------------------------------------------------------------

//{GlobalVars------------------------------------------------------------------
//-----------------------------------------------------------------------------

typedef sf::Vector2i Vector;

//-----------------------------------------------------------------------------

sf::RenderWindow* Window = nullptr;
float VMAX = 15;
const float EPS = 0.000001f;
//Vector Weight (1,1);
const float DEGREESINRADIAN = (float) (180/M_PI);


const sf::Vector2<int> MapSize (1600, 1000);
Vector  CameraPos (0, 0);
const Vector  WindowSize (1600, 1000);

const int NStars = 10;
const int NBullets = 10;

std::vector <Bullet*> BulletStorage;

//}
//-----------------------------------------------------------------------------


//{Prototypes------------------------------------------------------------------


struct Object
	{
	enum  STATUS {ALIVE, WAITINGFORDEATH};
	const STATUS status_;

	enum TYPE {UNDEFINED, MC,  STAR, BULLET, FON};
	const TYPE type_;

	Vector screenPos_;
	Vector mapPos_;
	Vector v_;

	sf::Sprite sprite_;
	//--------------------
	Object ();
	Object (const TYPE &type,
            const Vector &pos,
            const sf::Texture &tex,
            const sf::IntRect &rectangle);
    virtual ~Object () {};

	virtual void logic ();
	virtual void update();
	virtual void draw  ();
      bool tagged_dead();
	};

struct Collideable  : Object
    {
    float hitbox_;      //todo class hitbox in object class
    //--------------------
    Collideable (const TYPE &type,
                 const Vector &mapPos,
                 const sf::Texture &tex,
                 const sf::IntRect &rectangle,
                    float hitbox);

    virtual void collide () {};
    virtual void draw_according_to_round_map();

    };

struct  Shootable   : Collideable
    {
    //---------------------
    Shootable (const TYPE &type,
               const Vector &mapPos,
               const sf::Texture &tex,
               const sf::IntRect &rectangle,
                float hitbox);
    virtual void shoot ();
    };


struct      Fon     : Object
    {
    //----------------------
    Fon (const sf::Texture &tex);

    //virtual void logic () override;  достаточно простой класс, еле отличается от шаблона
    virtual void update() override;
    };

struct     Star     : Collideable
    {
    //-------------------
     Star (const Vector &pos,
                  const sf::Texture &tex,
                  const sf::IntRect &rectangle,
                    float hitbox);

    virtual void collide () override;

    virtual void logic () override;
	virtual void update() override;
	virtual void draw  () override;
    };

struct    Bullet    : Collideable
    {
    bool in_use_;
	//--------------------
    Bullet (const sf::Texture &tex);
    Bullet (const Vector &pos,
            const sf::Texture &tex,
            const sf::IntRect &rectangle,
                float hitbox,
                    bool in_use);

    virtual void collide () override;       //пока я не заметил что забыл орпределить эту функцию, undef reference to vtable for bullet

    //virtual void logic () override;
    virtual void draw  () override;
    virtual void update() override;
    void activate (Shootable* p_ship);
	};

struct      Mc      : Shootable
	{
	int rotation_;
	//--------------------
    Mc (const Vector &pos,
        const sf::Texture &tex,
        const sf::IntRect &rectangle,
          float hitbox,
          int rotation = 0);

    virtual void collide () override;

    virtual void logic () override;
    virtual void draw  () override;
    virtual void update() override;
            void control();
	};



struct Engine
    {
    std::vector <Object*> Objects_;
    std::vector <Fon*> Fons_;
    //-------------------
    Engine ();
   ~Engine () {};

    void add (Object* pls_take_care_of_me_senpai);
    void run  ();
    void let_all_the_poor_enter_valhalla();   //destruct
    void KillTheTagged();
    };

//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------


void check_for_events ();
void lowing_game_speed ();
Vector vNormalize (Vector longV);
const sf::IntRect TheWholeTextureRect (const sf::Texture &tex);
double hypot (const Vector& v1, const Vector& v2);

void SpawnStars (Engine* p_engine, const sf::Texture &p_star_tex, sf::Vector2 <int> starSizeRange, int nStars);
void SpawnBullets (const sf::Texture &p_bullet_txtr, int nBullets);

void WritePlayerCords (const Mc* player);
Object* CheckCollision(Engine* engine, Vector pos, float radius);

bool operator == (const Vector& lvalue, const sf::Vector2u& rvalue);
bool operator == (const sf::Vector2u& lvalue, const Vector& rvalue);
bool operator == (const sf::IntRect& lvalue, const sf::IntRect& rvalue);
Vector operator % (const Vector& lvalue, const  sf::Vector2<int>& rvalue);
Vector operator % (const Vector& lvalue, const      Vector&       rvalue);
Vector operator * (const Vector& lvalue, const int multiple);
//float  operator % (const float&  lvalue, const      float&        rvalue);     CYKA XYLU

void temp_game_proc (const sf::Texture &fon_tex,
                     const sf::Texture &player_tex,
                     const sf::Texture &star_tex,
                     const sf::Texture &bullet_txtr);

//{templates-------------------------------------------------------------------

template <typename T, typename U>
sf::Vector2 <T> operator / (const sf::Vector2 <T>& lvalue, const U& rvalue)
    {
    return sf::Vector2 <T> (lvalue.x / rvalue, lvalue.y / rvalue);
    }
/*
template <typename T, typename U>
float  operator % (const T lvalue, const U rvalue)
    {
    return lvalue % rvalue;
    }                                                                     DDA BLYAT'
float operator % (const float& lvalue, const float& rvalue)
    {
    float mlt = 1000000;
    return float ( ((int) lvalue*mlt % (int) rvalue*mlt)/mlt );

    }     */
//}
//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------


//===================================================================================================


//{Classes::-------------------------------------------------------------------

//{ Object::-------------------------------------------------------------------

Object::Object (const TYPE &type,
                const Vector &mapPos,
                const sf::Texture &tex,
                const sf::IntRect &rectangle) :
    status_ (ALIVE),
    type_ (type),
	mapPos_(mapPos),
	screenPos_(WindowSize/2),//временно, потом update изменит, а если рисоваться буит в центре экрана я пойму что забыл переопределить update в наследственном классе
	sprite_(tex, rectangle)
	{
	sprite_.setOrigin(float (rectangle.width/2) , float (rectangle.height/2) );
	}

//-----------------------------------------------------------------------------

Object::Object () :
    status_ (ALIVE),
    type_ (UNDEFINED),
    mapPos_(MapSize/2),
    screenPos_(mapPos_),
	sprite_()
	{
	}

//=============================================================================

void Object::logic()
    {}

//-----------------------------------------------------------------------------

void Object::update()
	{
	/*mapPos_ += v_;
	screenPos_ = mapPos_ - CameraPos;                        // типо шаблон простейшего движения, сотру как-нить
	sprite_.setPosition (screenPos_); */
	}

//-----------------------------------------------------------------------------

void Object::draw()
	{

	Window->draw(sprite_);
	}

//-----------------------------------------------------------------------------

bool Object::tagged_dead()
    {
    if (status_ == WAITINGFORDEATH) return true;
    return false;
    }

//}----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

//{     Collideable::----------------------------------------------------------

Collideable::Collideable (const TYPE &type,
                          const Vector &mapPos,
                          const sf::Texture &tex,
                          const sf::IntRect &rectangle,
                            float hitbox) :
Object (type, mapPos, tex, rectangle),
hitbox_ (hitbox)
    {}


//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------

void Collideable::draw_according_to_round_map()
    {
    Window->draw(sprite_);

    sf::Vector2<int> moveprg (0, 0);

    if (CameraPos.x + WindowSize.x/2 >  MapSize.x/2) moveprg.x = +1;
    if (CameraPos.x - WindowSize.x/2 < -MapSize.x/2) moveprg.x = -1;
    if (CameraPos.y + WindowSize.y/2 >  MapSize.y/2) moveprg.y = +1;
    if (CameraPos.y - WindowSize.y/2 < -MapSize.y/2) moveprg.y = -1;

    int times_to_draw = abs(moveprg.x) + abs(moveprg.y);
    if (times_to_draw)
        {
        sf::Sprite fakeSprite (sprite_);

        fakeSprite.setPosition(screenPos_.x + MapSize.x*moveprg.x, screenPos_.y + MapSize.y*moveprg.y);
        Window->draw(fakeSprite);
        if (times_to_draw == 2)
            {
            fakeSprite.setPosition(screenPos_.x, screenPos_.y + MapSize.y*moveprg.y);
            Window->draw(fakeSprite);
            fakeSprite.setPosition(screenPos_.x + MapSize.x*moveprg.x, screenPos_.y);
            Window->draw(fakeSprite);
            }
        }
    CameraPos = CameraPos % MapSize;
	}


//}
//-----------------------------------------------------------------------------

//{         Shootable::--------------------------------------------------------

Shootable::Shootable (const TYPE &type,
                      const Vector &mapPos,
                      const sf::Texture &tex,
                      const sf::IntRect &rectangle,
                        float hitbox) :

Collideable (type, mapPos, tex, rectangle, hitbox)
    {}

//=============================================================================

void Shootable::shoot ()
    {
    if (sf::Mouse::isButtonPressed(sf::Mouse::Left) )
        {
        for (auto& p_bul : BulletStorage)
            {
            if (!p_bul->in_use_) {p_bul->activate(this); return;}
            }
        printf ("sorry guy, no more bullets, try to play without\n");
        }
    }

//}
//-----------------------------------------------------------------------------


//{Fon::-----------------------------------------------------------------------

Fon::Fon (const sf::Texture &tex) :
    Object (FON, Vector(0, 0), tex, TheWholeTextureRect(tex))
    {}

//=============================================================================

//-----------------------------------------------------------------------------

void Fon::update ()
    {
    screenPos_ = mapPos_ /*- CameraPos/2* пока что так*/;
	sprite_.setPosition ((sf::Vector2f) screenPos_);
    }

//}
//-----------------------------------------------------------------------------

//{Star::----------------------------------------------------------------------

Star::Star (const Vector &mapPos,
            const sf::Texture &tex,
            const sf::IntRect &rectangle,
                float hitbox)         :
    Collideable (STAR, mapPos, tex, rectangle, hitbox)
    {
    auto rect = sprite_.getLocalBounds();
    sprite_.setOrigin(rect.width/2, rect.height/2);
    }

//=============================================================================


void Star::collide ()
    {}


//-----------------------------------------------------------------------------

void Star::logic ()
    {}

//-----------------------------------------------------------------------------

void Star::update()
    {
    Vector defaultDif (WindowSize/2);


	screenPos_ = defaultDif + mapPos_ - CameraPos;
    sprite_.setPosition ((sf::Vector2f) screenPos_);
    }

//-----------------------------------------------------------------------------

void Star::draw()
	{
    draw_according_to_round_map();
	}

//}
//-----------------------------------------------------------------------------

//{Bullet::--------------------------------------------------------------------
Bullet::Bullet (const Vector &mapPos,
                const sf::Texture &tex,
                const sf::IntRect &rectangle,
                    float hitbox,
                        bool in_use)         :
    Collideable (BULLET, mapPos, tex, rectangle, hitbox),
    in_use_ (in_use)
    {}

//-----------------------------------------------------------------------------

Bullet::Bullet (const sf::Texture &tex) :
    Collideable (BULLET, Vector(0, 0), tex, TheWholeTextureRect(tex), float (2)),
    in_use_ (false)
    {}

//=============================================================================

void Bullet::collide ()
    {}

//-----------------------------------------------------------------------------
void Bullet::update ()
    {
    Vector defdif = (WindowSize/2);
    mapPos_ += v_;
	screenPos_ = defdif + mapPos_ - CameraPos;
	sprite_.setPosition ((sf::Vector2f) screenPos_);
	}

//-----------------------------------------------------------------------------

void Bullet::draw ()
    {
    draw_according_to_round_map();
    }

//-----------------------------------------------------------------------------
void Bullet::activate(Shootable* p_ship)
    {
    in_use_ = true;
    v_ = vNormalize (p_ship->v_)*100;           //speed
    mapPos_ = p_ship->mapPos_;

    }

//}
//-----------------------------------------------------------------------------

//{Mc::------------------------------------------------------------------------

Mc::Mc  (const Vector &mapPos,
         const sf::Texture &tex,
         const sf::IntRect &rectangle,
           float hitbox,
           int rotation) :
	Shootable (MC, mapPos, tex, rectangle, hitbox),

	rotation_ (rotation)
	{}

//=============================================================================

void Mc::collide ()
    {
    //Collideable* p_potentialCollider = checkCollision();
    //if (p_potentialCollider) p_potentialCollider->collide(this);
    }

//-----------------------------------------------------------------------------

/*void Mc::collide (Mc* p_collider)
    {}

void Mc::collide (Star* p_collider)
    {}
*/
//-----------------------------------------------------------------------------

void Mc::control()
	{

	if (sf::Keyboard::isKeyPressed (sf::Keyboard::A))  v_.x -= 5;
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::D))  v_.x += 5;
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::W))  v_.y -= 5;
	if (sf::Keyboard::isKeyPressed (sf::Keyboard::S))  v_.y += 5;

    if (sf::Keyboard::isKeyPressed (sf::Keyboard::Space))  v_ = Vector (0, 0);

	if (v_.x >  VMAX) v_.x =  VMAX;
	if (v_.y >  VMAX) v_.y =  VMAX;
	if (v_.x < -VMAX) v_.x = -VMAX;
	if (v_.y < -VMAX) v_.y = -VMAX;

    v_ = (Vector) v_ * float (0.9);
    // if (!(!v.x && !v.y))         if (!!v.x || !!v.y)

    if (fabs (v_.x) > EPS || fabs (v_.y) > EPS)
        {
        Vector NormalV = vNormalize(v_);
        rotation_ = int (
                        (NormalV.y >= 0)?
                         DEGREESINRADIAN * (acos ( NormalV.x) ) :
                         DEGREESINRADIAN * (acos (-NormalV.x) ) + 180
                        );
        }

	}

//-----------------------------------------------------------------------------
void Mc::logic()
    {
    control();
    shoot();
    WritePlayerCords (this);
    }

//-----------------------------------------------------------------------------

void Mc::update()
	{
	int mult = 1000000;
    CameraPos += v_;
    mapPos_ += v_;

    if (abs (mapPos_.x) >  MapSize.x/2)   mapPos_.x = - (int (mapPos_.x) % MapSize.x);
    if (abs (mapPos_.y) >  MapSize.y/2)   mapPos_.y = - (int (mapPos_.y) % MapSize.y);
	screenPos_ = WindowSize/2;
	sprite_.setPosition ((sf::Vector2f) screenPos_);
	sprite_.setRotation(float(rotation_));
	}

//-----------------------------------------------------------------------------

void Mc::draw()
    {

    Window->draw(sprite_);
    }

//}
//-----------------------------------------------------------------------------


//{Engine::--------------------------------------------------------------------

Engine::Engine ()
    {}


//-----------------------------------------------------------------------------

void Engine::add (Object* p_obj)
    {
    assert(p_obj);

    if (p_obj->type_ == Object::FON) Fons_.push_back(static_cast <Fon*> (p_obj));
    else Objects_.push_back(p_obj);
    }

//-----------------------------------------------------------------------------

void Engine::run  ()
    {
    lowing_game_speed();
    check_for_events();

    Window->clear();

    KillTheTagged(); //мусорщик для обьектов в динамической памяти

    for (auto& p_fon : Fons_)
        {                       //отдельный массив для фонов, чтобы не делать исключения на checkcolission, хотя это можно сделать,
                                //и я так позже сделаю, когда буду пилить столкновения  todo 1 array    а может и не стоит, чтобы зря функции не вызывать
        p_fon->update();
        p_fon->draw();
        }

    for (auto& p_bul : BulletStorage)
        {
        if (!p_bul->in_use_) break;

        p_bul->update();
        p_bul->draw();
        }

    for (auto& p_obj : Objects_)
        {
        p_obj->logic();
        p_obj->update();
        p_obj->draw();
        //p_obj checkcollision   todo

        }

    Window->display();
    }

//-----------------------------------------------------------------------------
void Engine::KillTheTagged()
    {
    int size = Objects_.size();
    for (int i = 0; i < size; i++)
        {

        if (Objects_[i]->tagged_dead())
            {
            if (Objects_[i]->type_ == Object::STAR) delete Objects_[i];
            if (i != size - 1) Objects_.at(i) = Objects_.back();
            Objects_.pop_back ();
            i--;
            }
        }
    }

//-----------------------------------------------------------------------------

void Engine::let_all_the_poor_enter_valhalla()   //destruct
    {
    for (auto& p_obj : Objects_)
        {
        if (p_obj->type_ == Object::STAR) delete p_obj;
        }
    Objects_.clear ();
    //bullets?!

    }

//}
//-----------------------------------------------------------------------------


//}
//-----------------------------------------------------------------------------

//{Functions-------------------------------------------------------------------


int main()
	{
	srand(unsigned (time(nullptr)));
	sf::RenderWindow window (sf::VideoMode (WindowSize.x, WindowSize.y), "okoshe4ko");
	Window = &window;

    sf::Texture player_txtr;
    sf::Texture   star_txtr;
    sf::Texture    fon_txtr;
    sf::Texture bullet_txtr;

    star_txtr.loadFromFile   (/*"stars1.png")*/ "byablo4ko.bmp");
    fon_txtr.loadFromFile    ("fon1.png");
	player_txtr.loadFromFile ("shep1.png");
    bullet_txtr.loadFromFile ("byablo4ko_little.bmp");

	temp_game_proc (fon_txtr, player_txtr, star_txtr, bullet_txtr);
	}

//-----------------------------------------------------------------------------

void temp_game_proc (const sf::Texture &fon_tex,
                     const sf::Texture &player_tex,
                     const sf::Texture &star_tex,
                     const sf::Texture &bullet_txtr)
	{
    Engine engine;

	Mc player (Vector(0, 0), player_tex,   TheWholeTextureRect (player_tex), float(6666/*пока что хитбоксы не испоьзуются и собсна не запилены*/) );
	Fon fon (fon_tex);

    SpawnStars   (&engine, star_tex, sf::Vector2<int>(15, 30), NStars);
    SpawnBullets (bullet_txtr, NBullets);
    //SpawnTheEnemysheps (&engine, NSheps....);  todo class npc : staticobject and rename it

    engine.add (&player);
    engine.add (&fon);

	while (Window->isOpen() )
		{
		engine.run();
		}

    engine.let_all_the_poor_enter_valhalla();
	}

//-----------------------------------------------------------------------------

void SpawnStars (Engine* p_engine, const sf::Texture &p_star_tex, sf::Vector2 <int> starSizeRange, int nStars)
    {

    for (int i = 0, limit = 0; i < nStars && limit < nStars+100; i++, limit++)
        {

        Vector randPos = Vector (rand() % (MapSize.x + 1) - MapSize.x/2,
                                 rand() % (MapSize.y + 1) - MapSize.y/2);


        Star* meteor = new Star (randPos, p_star_tex, TheWholeTextureRect (p_star_tex), (float) 7);

        if (CheckCollision (p_engine, meteor->mapPos_, meteor->hitbox_)) { i--; continue; }

        p_engine->add (meteor);
        }
    }

//-----------------------------------------------------------------------------

void SpawnBullets (const sf::Texture &p_bullet_txtr, int nBullets)
    {

    for (int i = 0; i < nBullets; i++)
        BulletStorage.push_back (new Bullet(p_bullet_txtr));
    }

//-----------------------------------------------------------------------------

Object* CheckCollision(Engine* p_engine, Vector pos, float hitbox)
    {
    for (auto& p_object : p_engine->Objects_)
        {
        if (hypot(p_object->mapPos_, pos) < 50) return p_object;
        }
    return NULL;
    }

//-----------------------------------------------------------------------------

const sf::IntRect TheWholeTextureRect (const sf::Texture &tex)
    {

    auto size2u = tex.getSize();
    Vector size2 (size2u);
    return sf::IntRect (0, 0, size2.x, size2.y);
    }

//-----------------------------------------------------------------------------

void WritePlayerCords (const Mc* p_player)
    {
    std::string str_delta = "deltaX:" + std::to_string((int) CameraPos.x) + "\n" +
                            "deltaY:" + std::to_string((int) CameraPos.y);
    sf::Font font;
    font.loadFromFile ("C:/windows/fonts/Arial.ttf");

    sf::Text txt_delta (str_delta, font);
    Vector leftDownCornerPos ((float) 5, (float) 0.9*WindowSize.y);
    txt_delta.setFillColor (sf::Color (170, 60, 60));
    txt_delta.setPosition  ((sf::Vector2f) leftDownCornerPos);

    Window->draw (txt_delta);

    std::string str_cords = "posX:" + std::to_string((int) p_player->mapPos_.x) + "\n" +
                            "posY:" + std::to_string((int) p_player->mapPos_.y);


    sf::Text txt_cords (str_cords, font);
    Vector rightDownCornerPos ((float) WindowSize.x*0.5, (float) 0.9*WindowSize.y);
    txt_cords.setFillColor (sf::Color (170, 60, 60));
    txt_cords.setPosition  ((sf::Vector2f) rightDownCornerPos);

    Window->draw (txt_cords);

    }


//{service_func----------------------------------------------------------------

void check_for_events()
	{
	sf::Event event;
	while (Window->pollEvent (event) )
		if (event.type == sf::Event::Closed) Window->close();
	}

//-----------------------------------------------------------------------------

void lowing_game_speed()
    {
    Window->setFramerateLimit(15);
    //Sleep(100);
    }

//-----------------------------------------------------------------------------

double hypot (const Vector& v1, const Vector& v2)
    {
    return hypot(v1.x - v2.x, v1.y - v2.y);
    }

//-----------------------------------------------------------------------------

Vector vNormalize(Vector longV)
    {
    if (longV.x == 0 && longV.y == 0) return Vector (0, 1);
    return longV / hypot (longV.x, longV.y);
    }

//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------


//{operators-------------------------------------------------------------------

bool operator == (const Vector& lvalue, const sf::Vector2u& rvalue)
    {
    return ( lvalue.x == rvalue.x &&
             lvalue.y == rvalue.y );
    }

//-----------------------------------------------------------------------------

bool operator == (const sf::Vector2u& rvalue, const Vector& lvalue)
    {
    return lvalue == rvalue;
    }

//-----------------------------------------------------------------------------

bool operator == (const sf::IntRect& lvalue, const sf::IntRect& rvalue)
    {
    return ( lvalue.top    == rvalue.top   &&
             lvalue.left   == rvalue.left  &&
             lvalue.width  == rvalue.width &&
             lvalue.height == rvalue.height );
    }

//-----------------------------------------------------------------------------

sf::Vector2<int> operator % (const sf::Vector2<int>& lvalue, const sf::Vector2<int>& rvalue)
    {
    return sf::Vector2<int>(lvalue.x % rvalue.x,
           lvalue.y % rvalue.y);
    }

//-----------------------------------------------------------------------------

/*Vector operator % (const Vector& lvalue, const sf::Vector2<int>& rvalue)
    {
    float to4nost = 1000000;
    Vector ret (( (int) (lvalue.x * to4nost) % (int) (rvalue.x * to4nost)) / to4nost,
                ( (int) (lvalue.y * to4nost) % (int) (rvalue.y * to4nost)) / to4nost   );
    return   ret;
    }
         */
//-----------------------------------------------------------------------------

/*Vector operator % (const Vector& lvalue, const Vector& rvalue)
    {
    float to4nost = 10000;
    Vector ret (( (int) (lvalue.x * to4nost) % (int) (rvalue.x * to4nost)) / to4nost,
                ( (int) (lvalue.y * to4nost) % (int) (rvalue.y * to4nost)) / to4nost   );
    return   ret;
    }
         */
Vector operator * (const Vector& lvalue, const int multiple)
    {
    return Vector (lvalue.x * multiple, lvalue.y * multiple);
    }

//-----------------------------------------------------------------------------
/*float operator % (const float& lvalue, const float& rvalue)
    {
    float mlt = 1000000;
    return float ( ((int) lvalue*mlt % (int) rvalue*mlt)/mlt );

    }                    */
//}
//-----------------------------------------------------------------------------

//}
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------
//-----------------------------------------------------------------------------



















