//Including libraries
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

//--------------------------------------
// Structure
//-------------------------------------


//Function to draw the structure
void makeStructure(){

    //Setting background
    ClearBackground(SKYBLUE);

    //Drawing Ground
    DrawPlane((Vector3){0.0f, 1.0f, 0.f}, (Vector2){200.0f, 200.0f}, DARKGREEN);


};


// -------------------------------
// Main function
// -------------------------------


int main(void) {

  //Creating window
  const int windowHeight = 860;
  const int windowWidth = 840;
  InitWindow(windowWidth, windowHeight, "Jump3r");


  //-----------------------------------
  //Camera
  //-----------------------------------

  //Setting up camera
  Camera3D camera = { 0 };
  camera.position = (Vector3){10.0f, 2.0f, 10.0f}; //Plotter I used for help https://technology.cpm.org/general/3dgraph/
  camera.target = (Vector3){0.0f, 0.0f, 0.0f};   //What point camera is looking at
  camera.up = (Vector3){0.0f, 1.0f, 0.0f};
  camera.fovy = 45.0f;
  camera.projection = CAMERA_PERSPECTIVE;

  //Extra camera/window stuff
  DisableCursor();
  SetTargetFPS(60);
  Vector3 prevCameraPosition = camera.position;



  //Movement variables
  float walkingVelocity = 0.1f;
  float runningVelocity = 3.0f;
  float jumpVelocity = 8.0f; //Jump velocity
  const float gravity = -9.81f;
  bool isJumping = false;






    bool collision = false;

    Vector3 playerPosition = {10.0f, 1.5f, 10.0f};
    Vector3 playerSize = {0.5f, 0.5f, 0.5f};
    Color playerColor = WHITE;


    Vector3 testCubePosition = {0.0f, 2.0f, 0.0f,};
    Vector3 testCubeDimensions = {1.0f, 1.0f, 1.0f};








  //Main game loop
  while(!WindowShouldClose()){


      //-------------------------------
      // Camera
      //-------------------------------

      //Updating camera, allowing movement etc
      UpdateCameraPro(&camera,
                      (Vector3){
                              (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))*walkingVelocity -      // Move forward-backward
                              (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))*walkingVelocity,
                              (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))*walkingVelocity -   // Move right-left
                              (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))*walkingVelocity,
                              0.0f                                                // Move up-down
                      },
                      (Vector3){
                              GetMouseDelta().x*0.05f,                            // Rotation: yaw
                              GetMouseDelta().y*0.05f,                            // Rotation: pitch
                              0.0f                                                // Rotation: roll
                      },
                      GetMouseWheelMove()*2.0f);                              // Move to target (zoom)


      //Printing camera data to terminal
      //If any camera coord isn't equal to the previous camera coord then print new camera data, this is so there isn't constant camera coord spam from just printing coords on a loop
      if((camera.position.x != prevCameraPosition.x) || (camera.position.y != prevCameraPosition.y) || (camera.position.z != prevCameraPosition.z) ){
          printf("| X = %f | Y = %f | Z = %f ", camera.position.x, camera.position.y,
                 camera.position.z);
          prevCameraPosition = camera.position;

      }


     //------------------------------------
     // Jump Movement
     //-----------------------------------

      //If player presses space, they jump
      if(IsKeyPressed(KEY_SPACE)){
          isJumping = true;

      }


      //Elevate players camera y level
      if((isJumping)){

        //Adding gravity to jump velocity
        jumpVelocity += (gravity * GetFrameTime());
        //Elevating players y cam level
        camera.position.y += (jumpVelocity * GetFrameTime());
        camera.target.y += (jumpVelocity * GetFrameTime());

        playerPosition.y += (jumpVelocity * GetFrameTime());


        printf("Jump Velocity is %f", jumpVelocity);


      }

      //If camera pos is below the ground y level,
      if((camera.position.y) < 2.0f){

          //Player no longer jumping, reset jump velocity back to its initial value
          //and reset cameras positions back to its ground y level.
          isJumping = false;
          jumpVelocity = 8.0f;
          camera.position.y = 2.0f;
          printf("Jump Velocity is %f", jumpVelocity);

      }

      if(playerPosition.y < 1.5f){

          playerPosition.y = 1.5f;

      }









      //--------------------------------------------
      // Collision Movement
      //--------------------------------------------

      //Collision Box
      Vector3 testCubeCollision = {testCubePosition.x - testCubeDimensions.x/2,
                                   testCubePosition.y - testCubeDimensions.y/2,
                                   testCubePosition.z - testCubeDimensions.z/2
      };


      //Checking collision camera vs testCube
      if(CheckCollisionBoxes((BoundingBox){testCubeCollision},(BoundingBox){camera.position})){
        collision = true;
}

      if(collision){
          printf("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
      }





      //----------------------------------
      // Drawing
      //---------------------------------

      //Begin Draw
      BeginDrawing();


      //Starting "camera session" and calling makeStructure to populate the window with structures
      BeginMode3D(camera);

      //Draw player object
      DrawCylinder(playerPosition, playerSize.x, playerSize.y, playerSize.z, 20, playerColor);
      DrawCylinderWires(playerPosition, playerSize.x, playerSize.y, playerSize.z, 20, BLACK);



      makeStructure();
      DrawCubeV(testCubePosition, testCubeDimensions, RED);
      DrawCubeWiresV(testCubePosition, testCubeDimensions, WHITE);


      //Ending Session
      EndMode3D();

      //End Draw
      EndDrawing();


  }

    CloseWindow();

}
