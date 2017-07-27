#include <string>
#include <sstream>

#include <Urho3D/Core/CoreEvents.h>
#include <Urho3D/Engine/Application.h>
#include <Urho3D/Engine/Engine.h>
#include <Urho3D/Input/Input.h>
#include <Urho3D/Input/InputEvents.h>
#include <Urho3D/Resource/ResourceCache.h>
#include <Urho3D/Resource/XMLFile.h>
#include <Urho3D/IO/Log.h>
#include <Urho3D/UI/UI.h>
#include <Urho3D/UI/Text.h>
#include <Urho3D/UI/Font.h>
#include <Urho3D/UI/Button.h>
#include <Urho3D/UI/UIEvents.h>
#include <Urho3D/Scene/Scene.h>
#include <Urho3D/Scene/SceneEvents.h>
#include <Urho3D/Graphics/Graphics.h>
#include <Urho3D/Graphics/Camera.h>
#include <Urho3D/Graphics/Geometry.h>
#include <Urho3D/Graphics/Renderer.h>
#include <Urho3D/Graphics/DebugRenderer.h>
#include <Urho3D/Graphics/Octree.h>
#include <Urho3D/Graphics/Light.h>
#include <Urho3D/Graphics/Model.h>
#include <Urho3D/Graphics/StaticModel.h>
#include <Urho3D/Graphics/Material.h>
#include <Urho3D/Graphics/Skybox.h>

using namespace Urho3D;
class MyApp : public Application
{
public:
    int framecount_;
    float time_;
    SharedPtr<Text> text_;
    SharedPtr<Scene> scene_;
    SharedPtr<Node> boxNode_;
    SharedPtr<Node> cameraNode_;

    MyApp(Context* context) : Application(context)
    {
    }

    virtual void Setup()
    {
        // Called before engine initialization. engineParameters_ member variable can be modified here
        // These parameters should be self-explanatory.
        // See http://urho3d.github.io/documentation/1.5/_main_loop.html
        // for a more complete list.
        engineParameters_["FullScreen"]=false;
        engineParameters_["WindowWidth"]=1280;
        engineParameters_["WindowHeight"]=720;
        engineParameters_["WindowResizable"]=true;
    }

    virtual void Start()
    {
        // Called after engine initialization. Setup application & subscribe to events here
        SubscribeToEvent(E_KEYDOWN, URHO3D_HANDLER(MyApp, HandleKeyDown));
        SubscribeToEvent(E_UPDATE,URHO3D_HANDLER(MyApp,HandleUpdate));

        // We will be needing to load resources.
        // All the resources used in this example comes with Urho3D.
        // If the engine can't find them, check the ResourcePrefixPath (see http://urho3d.github.io/documentation/1.5/_main_loop.html).
        //ResourceCache* cache = GetSubsystem<ResourceCache>();
    }

    virtual void Stop()
    {
        // Perform optional cleanup after main loop has terminated
    }

    void HandleKeyDown(StringHash eventType, VariantMap& eventData)
    {
        using namespace KeyDown;
        // Check for pressing ESC. Note the engine_ member variable for convenience access to the Engine object
        int key = eventData[P_KEY].GetInt();
        if (key == KEY_ESCAPE)
            engine_->Exit();
        if (key == KEY_F11)
            GetSubsystem<Graphics>()->ToggleFullscreen();
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
        float MOVE_SPEED=10.0f;
        // Mouse sensitivity as degrees per pixel
        const float MOUSE_SENSITIVITY=0.1f;

        if(time_ >=1)
        {
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
            text_->SetText(s);
            URHO3D_LOGINFO(s);     // this show how to put stuff into the log
            framecount_=0;
            time_=0;
        }

        /*
        // Rotate the box thingy.
        // A much nicer way of doing this would be with a LogicComponent.
        // With LogicComponents it is easy to control things like movement
        // and animation from some IDE, console or just in game.
        // Alas, it is out of the scope for our simple example.
        boxNode_->Rotate(Quaternion(8*timeStep,16*timeStep,0));

        Input* input=GetSubsystem<Input>();
        if(input->GetQualifierDown(1))  // 1 is shift, 2 is ctrl, 4 is alt
            MOVE_SPEED*=10;
        if(input->GetKeyDown('W'))
            cameraNode_->Translate(Vector3(0,0, 1)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('S'))
            cameraNode_->Translate(Vector3(0,0,-1)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('A'))
            cameraNode_->Translate(Vector3(-1,0,0)*MOVE_SPEED*timeStep);
        if(input->GetKeyDown('D'))
            cameraNode_->Translate(Vector3( 1,0,0)*MOVE_SPEED*timeStep);

        if(!GetSubsystem<Input>()->IsMouseVisible())
        {
            // Use this frame's mouse motion to adjust camera node yaw and pitch. Clamp the pitch between -90 and 90 degrees
            IntVector2 mouseMove=input->GetMouseMove();
            static float yaw_=0;
            static float pitch_=0;
            yaw_+=MOUSE_SENSITIVITY*mouseMove.x_;
            pitch_+=MOUSE_SENSITIVITY*mouseMove.y_;
            pitch_=Clamp(pitch_,-90.0f,90.0f);
            // Reset rotation and set yaw and pitch again
            cameraNode_->SetDirection(Vector3::FORWARD);
            cameraNode_->Yaw(yaw_);
            cameraNode_->Pitch(pitch_);
        }*/
    }
};
URHO3D_DEFINE_APPLICATION_MAIN(MyApp)
