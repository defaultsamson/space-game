#include <string>
#include <sstream>
#include <cmath>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Zone.h>
#include <Urho3D/Urho2D/CollisionBox2D.h>
#include <Urho3D/Urho2D/PhysicsWorld2D.h>
#include <Urho3D/Urho2D/RigidBody2D.h>
#include <Urho3D/Urho2D/StaticSprite2D.h>
#include <Urho3D/Urho2D/Sprite2D.h>
#include <Urho3D/Urho2D/SpriteSheet2D.h>


using namespace Urho3D;

/**
* Using the convenient Application API we don't have
* to worry about initializing the engine or writing a main.
* You can probably mess around with initializing the engine
* and running a main manually, but this is convenient and portable.
*/
class MyApp : public Application
{
public:

    bool debug = false;

    const float PPM = 100.0F; // pixels per meter
    const float MPP = 1.0F / PPM; // meters per pixel
    
    const float SHIP_FORCE = 10;
    const float ANGULAR_SHIP_FORCE = sqrt(SHIP_FORCE * SHIP_FORCE / 2);
    
    int framecount_;
    float time_;
    //SharedPtr<Text> text_;
    SharedPtr<Scene> scene_;
    //SharedPtr<Node> boxNode_;
    SharedPtr<Node> cameraNode_;
    
    RigidBody2D* shipBody_;
    SharedPtr<Node> shipNode_;
    PhysicsWorld2D* world;
    
    const int starsSize = 512;
    const int starsScale = 2;
    const int netStarSize = starsSize * starsScale;
    SharedPtr<Node> stars_;
    SharedPtr<Node> stars1_;
    SharedPtr<Node> stars2_;
    SharedPtr<Node> stars3_;
    
    //static const StringHash VAR_MOVESPEED("MoveSpeed");
    //static const StringHash VAR_ROTATESPEED("RotateSpeed");

    /**
    * This happens before the engine has been initialized
    * so it's usually minimal code setting defaults for
    * whatever instance variables you have.
    * You can also do this in the Setup method.
    */
    MyApp(Context * context) : Application(context),framecount_(0),time_(0)
    {
    }

    /**
    * This method is called before the engine has been initialized.
    * Thusly, we can setup the engine parameters before anything else
    * of engine importance happens (such as windows, search paths,
    * resolution and other things that might be user configurable).
    */
    virtual void Setup()
    {
        // These parameters should be self-explanatory.
        // See http://urho3d.github.io/documentation/1.5/_main_loop.html
        // for a more complete list.
        engineParameters_["FullScreen"]=false;
        engineParameters_["WindowWidth"]= 16 * 80; // 1280
        engineParameters_["WindowHeight"]= 9 * 80; // 720
        engineParameters_["WindowResizable"]=true;
        engineParameters_["WindowTitle"]="Space Game by Samson";
        engineParameters_["LogName"]="SpaceGame.log";
        // engineParameters_["WindowIcon"]="SpaceGame/"; // WindowIcon (string) Window icon image resource name. Default empty (use application default icon.)
        engineParameters_["VSync"]=false;
    }

    /**
    * This method is called after the engine has been initialized.
    * This is where you set up your actual content, such as scenes,
    * models, controls and what not. Basically, anything that needs
    * the engine initialized and ready goes in here.
    */
    virtual void Start()
    {
        // We will be needing to load resources.
        // All the resources used in this example comes with Urho3D.
        // If the engine can't find them, check the ResourcePrefixPath (see http://urho3d.github.io/documentation/1.5/_main_loop.html).
        ResourceCache* cache=GetSubsystem<ResourceCache>();
        
        // Get sprite
        Sprite2D* shipSprite = cache->GetResource<Sprite2D>("SpaceGame/ship.png");
        SpriteSheet2D* starsSheet = cache->GetResource<SpriteSheet2D>("SpaceGame/sheets/ambient.xml");
        
        /*
        // Let's use the default style that comes with Urho3D.
        GetSubsystem<UI>()->GetRoot()->SetDefaultStyle(cache->GetResource<XMLFile>("UI/DefaultStyle.xml"));
        // Let's create some text to display.
        text_=new Text(context_);
        // Text will be updated later in the E_UPDATE handler. Keep readin'.
        text_->SetText("Keys: tab = toggle mouse, AWSD = move camera, Shift = fast mode, Esc = quit.\nWait a bit to see FPS.");
        // If the engine cannot find the font, it comes with Urho3D.
        // Set the environment variables URHO3D_HOME, URHO3D_PREFIX_PATH or
        // change the engine parameter "ResourcePrefixPath" in the Setup method.
        text_->SetFont(cache->GetResource<Font>("Fonts/Anonymous Pro.ttf"),20);
        text_->SetColor(Color(.3,0,.3));
        text_->SetHorizontalAlignment(HA_CENTER);
        text_->SetVerticalAlignment(VA_TOP);
        GetSubsystem<UI>()->GetRoot()->AddChild(text_);
        // Add a button, just as an interactive UI sample.
        Button* button=new Button(context_);
        // Note, must be part of the UI system before SetSize calls!
        GetSubsystem<UI>()->GetRoot()->AddChild(button);
        button->SetName("Button Quit");
        button->SetStyle("Button");
        button->SetSize(32,32);
        button->SetPosition(16,116);
        
        // Subscribe to button release (following a 'press') events
        SubscribeToEvent(button,E_RELEASED,URHO3D_HANDLER(MyApp,HandleClosePressed));
        */

        // Let's setup a scene to render.
        scene_=new Scene(context_);
        // Let the scene have an Octree component!
        scene_->CreateComponent<Octree>();
        // Let's add an additional scene component for fun.
        scene_->CreateComponent<DebugRenderer>();

        // Sets background colour
        GetSubsystem<Renderer>()->GetDefaultZone()->SetFogColor(Color(0.1289F, 0.1016F, 0.1367F)); // #211a23

        /*
        // Let's put some sky in there.
        // Again, if the engine can't find these resources you need to check
        // the "ResourcePrefixPath". These files come with Urho3D.
        Node* skyNode=scene_->CreateChild("Sky");
        skyNode->SetScale(500.0f); // The scale actually does not matter
        Skybox* skybox=skyNode->CreateComponent<Skybox>();
        skybox->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        skybox->SetMaterial(cache->GetResource<Material>("Materials/Skybox.xml"));
        */

        /*
        // Let's put a box in there.
        boxNode_=scene_->CreateChild("Box");
        boxNode_->SetPosition(Vector3(0,2,15));
        boxNode_->SetScale(Vector3(3,3,3));
        StaticModel* boxObject=boxNode_->CreateComponent<StaticModel>();
        boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
        boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
        boxObject->SetCastShadows(true);
        */
        
        /*
        // Create 400 boxes in a grid.
        for(int x=-30;x<30;x+=3)
            for(int z=0;z<60;z+=3)
            {
                Node* boxNode_=scene_->CreateChild("Box");
                boxNode_->SetPosition(Vector3(x,-3,z));
                boxNode_->SetScale(Vector3(2,2,2));
                StaticModel* boxObject=boxNode_->CreateComponent<StaticModel>();
                boxObject->SetModel(cache->GetResource<Model>("Models/Box.mdl"));
                boxObject->SetMaterial(cache->GetResource<Material>("Materials/Stone.xml"));
                boxObject->SetCastShadows(true);
            }
        */

        // We need a camera from which the viewport can render.
        // Create camera node
        cameraNode_=scene_->CreateChild("Camera");
        // Set camera's position
        cameraNode_->SetPosition(Vector3(0.0f, 0.0f, -10.0f));
        // Create the camera componeny
        Camera* camera = cameraNode_->CreateComponent<Camera>();
        // Set the camera to orthographic (camera only moves across a 2D plane)
        camera->SetOrthographic(true);
        // Not 100% of the purpose behind this
        Graphics* graphics = GetSubsystem<Graphics>();
        camera->SetOrthoSize((float)graphics->GetHeight() * PIXEL_SIZE);
        
        
        
        world = scene_->CreateComponent<PhysicsWorld2D>();
        world->DrawDebugGeometry();/*
        world->SetDrawShape(true);
        world->SetDrawJoint(true);
        world->SetDrawAabb(true);
        world->SetDrawPair(true);
        world->SetDrawCenterOfMass(true);*/
        world->SetGravity(Vector2(0.0F, 0.0F));
        
        
        stars_ = scene_->CreateChild("Stars");
        {
            stars1_ = stars_->CreateChild("Stars1");
            stars1_->SetScale(starsScale);
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    Node* node = stars1_->CreateChild("");
                    node->SetPosition2D(starsSize * i * MPP, starsSize * j * MPP);
                    StaticSprite2D* staticSprite = node->CreateComponent<StaticSprite2D>();
                    staticSprite->SetLayer(0);
                    staticSprite->SetSprite(starsSheet->GetSprite("stars1"));
                    staticSprite->SetBlendMode(BLEND_ALPHA);
                }
            }
        }
        {
            stars2_ = stars_->CreateChild("Stars2");
            stars2_->SetScale(starsScale);
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    Node* node = stars2_->CreateChild("");
                    node->SetPosition2D(starsSize * i * MPP, starsSize * j * MPP);
                    StaticSprite2D* staticSprite = node->CreateComponent<StaticSprite2D>();
                    staticSprite->SetLayer(0);
                    staticSprite->SetSprite(starsSheet->GetSprite("stars2"));
                    staticSprite->SetBlendMode(BLEND_ALPHA);
                }
            }
        }
        {
            stars3_ = stars_->CreateChild("Stars3");
            stars3_->SetScale(starsScale);
            for(int i = -1; i <= 1; i++) {
                for(int j = -1; j <= 1; j++) {
                    Node* node = stars3_->CreateChild("");
                    node->SetPosition2D(starsSize * i * MPP, starsSize * j * MPP);
                    StaticSprite2D* staticSprite = node->CreateComponent<StaticSprite2D>();
                    staticSprite->SetLayer(0);
                    staticSprite->SetSprite(starsSheet->GetSprite("stars3"));
                    staticSprite->SetBlendMode(BLEND_ALPHA);
                }
            }
        }
        
        {
            shipNode_ = scene_->CreateChild("Ship");
            shipNode_->SetPosition(Vector3(0, 0.0F, 0.0f));

            int shipScale = 4;
            Node* shipSpriteNode_ = shipNode_->CreateChild("ShipSprite");
            shipSpriteNode_->SetScale(4);
            
            StaticSprite2D* staticSprite = shipSpriteNode_->CreateComponent<StaticSprite2D>();
            staticSprite->SetLayer(1);
            staticSprite->SetSprite(shipSprite);
            staticSprite->SetBlendMode(BLEND_ALPHA);

            // Create rigid body
            shipBody_ = shipNode_->CreateComponent<RigidBody2D>();
            shipBody_->SetBodyType(BT_DYNAMIC); // BT_DYNAMIC

            // Create box
            CollisionBox2D* box = shipNode_->CreateComponent<CollisionBox2D>();
            int shipHeight = 23 * shipScale;
            int shipImageHeight = 32 * shipScale;
            // Set size
            box->SetSize(Vector2(60 * MPP, shipHeight * MPP));
            // Set size
            box->SetCenter(Vector2(0.0f, ((shipHeight - shipImageHeight) / 2) * MPP));
            // Set density
            box->SetDensity(1.0f);
            // Set friction
            box->SetFriction(1.0f);
            // Set restitution
            box->SetRestitution(0.1f);
        }
        
        // Set random color
        //staticSprite->SetColor(Color(Random(1.0f), Random(1.0f), Random(1.0f), 1.0f));
        // Set blend mode
        //staticSprite->SetBlendMode(BLEND_ALPHA);
        //staticSprite->SetScale(5);
        

        // Set move speed
        //spriteNode->SetVar(VAR_MOVESPEED, Vector3(Random(-2.0f, 2.0f), Random(-2.0f, 2.0f), 0.0f));
        // Set rotate speed
        //spriteNode->SetVar(VAR_ROTATESPEED, Random(-90.0f, 90.0f));

        // Add to sprite node vector
        //spriteNodes_.Push(spriteNode);

        /*
        // Create a red directional light (sun)
        {
            Node* lightNode=scene_->CreateChild();
            lightNode->SetDirection(Vector3::FORWARD);
            lightNode->Yaw(50);     // horizontal
            lightNode->Pitch(10);   // vertical
            Light* light=lightNode->CreateComponent<Light>();
            light->SetLightType(LIGHT_DIRECTIONAL);
            light->SetBrightness(1.6);
            light->SetColor(Color(1.0,.6,0.3,1));
            light->SetCastShadows(true);
        }
        // Create a blue point light
        {
            Node* lightNode=scene_->CreateChild("Light");
            lightNode->SetPosition(Vector3(-10,2,5));
            Light* light=lightNode->CreateComponent<Light>();
            light->SetLightType(LIGHT_POINT);
            light->SetRange(25);
            light->SetBrightness(1.7);
            light->SetColor(Color(0.5,.5,1.0,1));
            light->SetCastShadows(true);
        }
        // add a green spot light to the camera node
        {
            Node* node_light=cameraNode_->CreateChild();
            Light* light=node_light->CreateComponent<Light>();
            node_light->Pitch(15);  // point slightly downwards
            light->SetLightType(LIGHT_SPOT);
            light->SetRange(20);
            light->SetColor(Color(.6,1,.6,1.0));
            light->SetBrightness(2.8);
            light->SetFov(25);
        }
        */
        
        /*
        // Now we setup the viewport. Of course, you can have more than one!
        Renderer* renderer=GetSubsystem<Renderer>();
        SharedPtr<Viewport> viewport(new Viewport(context_,scene_,cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0,viewport);
        */
        
        Renderer* renderer = GetSubsystem<Renderer>();

        // Set up a viewport to the Renderer subsystem so that the 3D scene can be seen
        SharedPtr<Viewport> viewport(new Viewport(context_, scene_, cameraNode_->GetComponent<Camera>()));
        renderer->SetViewport(0, viewport);
        
        GetSubsystem<Input>()->SetMouseVisible(true);
        GetSubsystem<Input>()->SetMouseGrabbed(false);

        // We subscribe to the events we'd like to handle.
        // In this example we will be showing what most of them do,
        // but in reality you would only subscribe to the events
        // you really need to handle.
        // These are sort of subscribed in the order in which the engine
        // would send the events. Read each handler method's comment for
        // details.
        SubscribeToEvent(E_BEGINFRAME,URHO3D_HANDLER(MyApp,HandleBeginFrame));
        SubscribeToEvent(E_UPDATE,URHO3D_HANDLER(MyApp,HandleUpdate));
        SubscribeToEvent(E_POSTUPDATE,URHO3D_HANDLER(MyApp,HandlePostUpdate));
        SubscribeToEvent(E_RENDERUPDATE,URHO3D_HANDLER(MyApp,HandleRenderUpdate));
        SubscribeToEvent(E_POSTRENDERUPDATE,URHO3D_HANDLER(MyApp,HandlePostRenderUpdate));
        SubscribeToEvent(E_ENDFRAME,URHO3D_HANDLER(MyApp,HandleEndFrame));
        
        SubscribeToEvent(E_KEYDOWN,URHO3D_HANDLER(MyApp,HandleKeyDown));
    }

    /**
    * Good place to get rid of any system resources that requires the
    * engine still initialized. You could do the rest in the destructor,
    * but there's no need, this method will get called when the engine stops,
    * for whatever reason (short of a segfault).
    */
    virtual void Stop()
    {
    }

    /**
    * Every frame's life must begin somewhere. Here it is.
    */
    void HandleBeginFrame(StringHash eventType,VariantMap& eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }

    /**
    * Input from keyboard is handled here. I'm assuming that Input, if
    * available, will be handled before E_UPDATE.
    */
    void HandleKeyDown(StringHash eventType,VariantMap& eventData)
    {
        using namespace KeyDown;
        int key=eventData[P_KEY].GetInt();
        if(key==KEY_ESCAPE)
            engine_->Exit();
        if (key == KEY_F11)
            GetSubsystem<Graphics>()->ToggleFullscreen();
        if (key == KEY_F10)
            debug = !debug;

        if(key==KEY_TAB)    // toggle mouse cursor when pressing tab
        {
            GetSubsystem<Input>()->SetMouseVisible(!GetSubsystem<Input>()->IsMouseVisible());
            GetSubsystem<Input>()->SetMouseGrabbed(!GetSubsystem<Input>()->IsMouseGrabbed());
        }
    }

    /**
    * You can get these events from when ever the user interacts with the UI.
    */
    void HandleClosePressed(StringHash eventType,VariantMap& eventData)
    {
        engine_->Exit();
    }
    /**
    * Your non-rendering logic should be handled here.
    * This could be moving objects, checking collisions and reaction, etc.
    */
    void HandleUpdate(StringHash eventType,VariantMap& eventData)
    {
        float timeStep=eventData[Update::P_TIMESTEP].GetFloat();
        framecount_++;
        time_+=timeStep;
        // Movement speed as world units per second
        float MOVE_SPEED=4.0f;
        // Mouse sensitivity as degrees per pixel
        const float MOUSE_SENSITIVITY=0.1f;

        if(time_ >=1)
        {
            /*
            std::string str;
            str.append("Keys: tab = toggle mouse, AWSD = move camera, Shift = fast mode, Esc = quit.\n");
            {
                std::ostringstream ss;
                ss<<framecount_;
                std::string s(ss.str());
                str.append(s.substr(0,6));
            }
            str.append(" frames in ");
            {
                std::ostringstream ss;
                ss<<time_;
                std::string s(ss.str());
                str.append(s.substr(0,6));
            }
            str.append(" seconds = ");
            {
                std::ostringstream ss;
                ss<<(float)framecount_/time_;
                std::string s(ss.str());
                str.append(s.substr(0,6));
            }
            str.append(" fps");
            String s(str.c_str(),str.size());
            //text_->SetText(s);
            URHO3D_LOGINFO(s);     // this is how to put stuff into the log*/
            framecount_=0;
            time_=0;
        }

        // Do not move if the UI has a focused element (the console)
        if (GetSubsystem<UI>()->GetFocusElement())
            return;

        Input* input = GetSubsystem<Input>();

        bool up = input->GetKeyDown(KEY_W) || input->GetKeyDown(KEY_UP);
        bool down = input->GetKeyDown(KEY_S) || input->GetKeyDown(KEY_DOWN);
        bool left = input->GetKeyDown(KEY_A) || input->GetKeyDown(KEY_LEFT);
        bool right = input->GetKeyDown(KEY_D) || input->GetKeyDown(KEY_RIGHT);
        
        if (right && up) 
        {
            shipNode_->SetRotation2D(315);
            shipBody_->ApplyForceToCenter(Vector2(ANGULAR_SHIP_FORCE, ANGULAR_SHIP_FORCE), true);
        }
        else if (right && down) 
        {
            shipNode_->SetRotation2D(225);
            shipBody_->ApplyForceToCenter(Vector2(ANGULAR_SHIP_FORCE, -ANGULAR_SHIP_FORCE), true);
        }
        else if (left && down) 
        {
            shipNode_->SetRotation2D(135);
            shipBody_->ApplyForceToCenter(Vector2(-ANGULAR_SHIP_FORCE, -ANGULAR_SHIP_FORCE), true);
        }
        else if (left && up) 
        {
            shipNode_->SetRotation2D(45);
            shipBody_->ApplyForceToCenter(Vector2(-ANGULAR_SHIP_FORCE, ANGULAR_SHIP_FORCE), true);
        }
        else if (up) 
        {
            shipNode_->SetRotation2D(0);
            shipBody_->ApplyForceToCenter(Vector2(0, SHIP_FORCE), true);
        }
        else if (right) 
        {
            shipNode_->SetRotation2D(270);
            shipBody_->ApplyForceToCenter(Vector2(SHIP_FORCE, 0), true);
        }
        else if (down) 
        {
            shipNode_->SetRotation2D(180);
            shipBody_->ApplyForceToCenter(Vector2(0, -SHIP_FORCE), true);
        }
        else if (left) 
        {
            shipNode_->SetRotation2D(90);
            shipBody_->ApplyForceToCenter(Vector2(-SHIP_FORCE, 0), true);
        }

        if (input->GetKeyDown(KEY_PAGEUP))
        {
            Camera* camera = cameraNode_->GetComponent<Camera>();
            camera->SetZoom(camera->GetZoom() * 1.01f);
        }

        if (input->GetKeyDown(KEY_PAGEDOWN))
        {
            Camera* camera = cameraNode_->GetComponent<Camera>();
            camera->SetZoom(camera->GetZoom() * 0.99f);
        }
    }
    
    /**
    * Anything in the non-rendering logic that requires a second pass,
    * it might be well suited to be handled here.
    */
    void HandlePostUpdate(StringHash eventType,VariantMap& eventData)
    {
        // Update camera and stars to follow the ship
        float lastX = cameraNode_->GetPosition().x_;
        float lastY = cameraNode_->GetPosition().y_;
        
        cameraNode_->SetPosition(shipNode_->GetPosition());
        stars_->SetPosition(shipNode_->GetPosition());
        
        float changeX = cameraNode_->GetPosition().x_ - lastX;
        float changeY = cameraNode_->GetPosition().y_ - lastY;
        
        {
            float x1 = stars1_->GetPosition().x_ - changeX + (changeX / 1.2F);
            float y1 = stars1_->GetPosition().y_ - changeY + (changeY / 1.2F);
            stars1_->SetPosition2D(Vector2(x1, y1));
            
            if (stars1_->GetPosition().x_ * PPM > netStarSize) {
                stars1_->SetPosition2D(Vector2(x1 - (netStarSize * MPP), y1));
            } else if (stars1_->GetPosition().x_ * PPM < -netStarSize) {
                stars1_->SetPosition2D(Vector2(x1 + (netStarSize * MPP), y1));
            }
            
            float x2 = stars1_->GetPosition().x_;
            
            if (stars1_->GetPosition().y_ * PPM > netStarSize) {
                stars1_->SetPosition2D(Vector2(x2, y1 - (netStarSize * MPP)));
            } else if (stars1_->GetPosition().y_ * PPM < -netStarSize) {
                stars1_->SetPosition2D(Vector2(x2, y1 + (netStarSize * MPP)));
            }
        }
        {
            float x1 = stars2_->GetPosition().x_ - changeX + (changeX / 1.3F);
            float y1 = stars2_->GetPosition().y_ - changeY + (changeY / 1.3F);
            stars2_->SetPosition2D(Vector2(x1, y1));
            
            if (stars2_->GetPosition().x_ * PPM > netStarSize) {
                stars2_->SetPosition2D(Vector2(x1 - (netStarSize * MPP), y1));
            } else if (stars2_->GetPosition().x_ * PPM < -netStarSize) {
                stars2_->SetPosition2D(Vector2(x1 + (netStarSize * MPP), y1));
            }

            float x2 = stars2_->GetPosition().x_;

            if (stars2_->GetPosition().y_ * PPM > netStarSize) {
                stars2_->SetPosition2D(Vector2(x2, y1 - (netStarSize * MPP)));
            } else if (stars2_->GetPosition().y_ * PPM < -netStarSize) {
                stars2_->SetPosition2D(Vector2(x2, y1 + (netStarSize * MPP)));
            }
        }
        {
            float x1 = stars3_->GetPosition().x_ - changeX + (changeX / 1.4F);
            float y1 = stars3_->GetPosition().y_ - changeY + (changeY / 1.4F);
            stars3_->SetPosition2D(Vector2(x1, y1));
            
            if (stars3_->GetPosition().x_ * PPM > netStarSize) {
                stars3_->SetPosition2D(Vector2(x1 - (netStarSize * MPP), y1));
            } else if (stars3_->GetPosition().x_ * PPM < -netStarSize) {
                stars3_->SetPosition2D(Vector2(x1 + (netStarSize * MPP), y1));
            }
            
            float x2 = stars3_->GetPosition().x_;
            
            if (stars3_->GetPosition().y_ * PPM > netStarSize) {
                stars3_->SetPosition2D(Vector2(x2, y1 - (netStarSize * MPP)));
            } else if (stars3_->GetPosition().y_ * PPM < -netStarSize) {
                stars3_->SetPosition2D(Vector2(x2, y1 + (netStarSize * MPP)));
            }
        }
        
    }
    
    /**
    * If you have any details you want to change before the viewport is
    * rendered, try putting it here.
    * See http://urho3d.github.io/documentation/1.32/_rendering.html
    * for details on how the rendering pipeline is setup.
    */
    void HandleRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        
    }
    /**
    * After everything is rendered, there might still be things you wish
    * to add to the rendering. At this point you cannot modify the scene,
    * only post rendering is allowed. Good for adding things like debug
    * artifacts on screen or brush up lighting, etc.
    */
    void HandlePostRenderUpdate(StringHash eventType, VariantMap & eventData)
    {
        // We could draw some debuggy looking thing for the octree.
        //scene_->GetComponent<Octree>()->DrawDebugGeometry(true);
        if (debug) world->DrawDebugGeometry();
    }
    /**
    * All good things must come to an end.
    */
    void HandleEndFrame(StringHash eventType,VariantMap& eventData)
    {
        // We really don't have anything useful to do here for this example.
        // Probably shouldn't be subscribing to events we don't care about.
    }
};

/**
* This macro is expanded to (roughly, depending on OS) this:
*
* > int RunApplication()
* > {
* > Urho3D::SharedPtr<Urho3D::Context> context(new Urho3D::Context());
* > Urho3D::SharedPtr<className> application(new className(context));
* > return application->Run();
* > }
* >
* > int main(int argc, char** argv)
* > {
* > Urho3D::ParseArguments(argc, argv);
* > return function;
* > }
*/
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)