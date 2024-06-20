//Including libraries
#include <raylib.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

//--------------------------------------
// Structure
//-------------------------------------


//Function to draw the structure
void makeGround(){

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
  Vector3 prevPlayerPosition;



  //Movement variables
  float walkingVelocity = 0.1f;
  float runningVelocity = 0.2f;
  float jumpVelocity = 8.0f; //Jump velocity
  const float gravity = -9.81f;
  bool isJumping = false;
  bool isRunning = false;





    bool collision = false;

//    Vector3 playerPosition = camera.position;
//    Vector3 playerSize = {0.5f, 0.5f, 0.5f};
//    Color playerColor = WHITE;


    Vector3 testCubePosition = {0.0f, 2.0f, 0.0f,};
    Vector3 testCubeDimensions = {1.0f, 1.0f, 1.0f};








  //Main game loop
  while(!WindowShouldClose()){

      //-------------------------------
      //Player Movement
      //------------------------------


      //Player variables
      float adjustedPlayerY = (camera.position.y - 0.5f);

      Vector3 playerPosition = {camera.position.x, adjustedPlayerY, camera.position.z};
      Vector3 playerSize = {1.0f, 1.0f, 1.0f};
      Color playerColor = WHITE;

      //Setting previous camera and player positions for collision purposes
      prevCameraPosition = camera.position;
      prevPlayerPosition = playerPosition;


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
//      if((camera.position.x != prevCameraPosition.x) || (camera.position.y != prevCameraPosition.y) || (camera.position.z != prevCameraPosition.z) ){
//          printf("| X = %f | Y = %f | Z = %f ", camera.position.x, camera.position.y,
//                 camera.position.z);
//
//      }





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

        //Elevate player pos y level
        playerPosition.y += (jumpVelocity * GetFrameTime());

//        printf("Jump Velocity is %f", jumpVelocity);


      }

      //If camera pos is below the ground y level,
      if((camera.position.y) < 2.0f){

          //Player no longer jumping, reset jump velocity back to its initial value
          //and reset cameras positions back to its ground y level.
          isJumping = false;
          jumpVelocity = 8.0f;
          camera.position.y = 2.0f;

      }

      //If player pos is below the eye level, reset to eye level
      if(playerPosition.y < 1.5f){
          playerPosition.y = 1.5f;
      }


      //---------------------------------------
      // Running Movement
      //---------------------------------------


      //If player is holding Lshift, they are running
      if((IsKeyDown(KEY_LEFT_SHIFT))){
            isRunning = true;
            printf("PLAYER IS RUNNING");
      }
      //If player is running, increase velocity
      if(isRunning){
          walkingVelocity += runningVelocity;
      }
      //Keeping running speed at 0.4f
      if((walkingVelocity > 0.3f) && isRunning){
          walkingVelocity = 0.3f;
      }
      //If player is not running, return to normal walking velocity
      if(IsKeyReleased(KEY_LEFT_SHIFT)){
          isRunning = false;
          walkingVelocity = 0.1f;
      }


//      prevCameraPosition = camera.position;
//      prevPlayerPosition = playerPosition;


      //--------------------------------------------
      // Collision
      //--------------------------------------------


      //Collision Box
      Vector3 testCubeCollisionNegative = {testCubePosition.x - testCubeDimensions.x/2,
                                   testCubePosition.y - testCubeDimensions.y/2,
                                   testCubePosition.z - testCubeDimensions.z/2
      };
      Vector3 testCubeCollisionPositive = {testCubePosition.x + testCubeDimensions.x/2,
                                           testCubePosition.y + testCubeDimensions.y/2,
                                           testCubePosition.z + testCubeDimensions.z/2
      };

      Vector3 playerColNeg = {playerPosition.x - playerSize.x/2,
                              playerPosition.y - playerSize.y/2,
                              playerPosition.z - playerSize.z/2};

      Vector3 playerColPos = {playerPosition.x + playerSize.x/2,
                              playerPosition.y + playerSize.y/2,
                              playerPosition.z + playerSize.z/2};


      //Player collides with test cube, collision = true, and print you collided!
      if(CheckCollisionBoxes((BoundingBox){testCubeCollisionNegative, testCubeCollisionPositive},(BoundingBox){playerColNeg, playerColPos})) {
          collision = true;
      } else {
          collision = false;
      }


      //If colliding, revert players position to the position RIGHT before impact
      if(collision){
          printf("COLLISION!");
//          playerPosition.x = 10.0f, //prevPlayerPosition.x;
//          playerPosition.y = 2.0f,//prevPlayerPosition.y;
//          playerPosition.z = 10.0f,//prevPlayerPosition.z;
//
//          camera.position.x = 10.0f, //prevCameraPosition.x;
//          camera.position.y = adjustedPlayerY, //prevCameraPosition.y;
//          camera.position.z = 10.0f, //prevCameraPosition.z;
//          collision = false;




      }




      //----------------------------------
      // Drawing
      //---------------------------------

      //Begin Draw
      BeginDrawing();


      //Starting "camera session" and calling makeStructure to populate the window with structures
      BeginMode3D(camera);

      //Draw player object
      DrawCubeV(playerPosition,playerSize, RED);
      DrawCubeWiresV(playerPosition, playerSize, BLACK);




      makeGround();
      DrawCubeV(testCubePosition, testCubeDimensions, RED);
      DrawCubeWiresV(testCubePosition, testCubeDimensions, WHITE);


      //Ending Session
      EndMode3D();

      //End Draw
      EndDrawing();


  }

    CloseWindow();

}
