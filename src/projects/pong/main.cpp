#include <iostream>
#include <cstdlib>
#include "../core/Starisk.h"
#include "../core/ecs/components.h"
#include "imgui/imgui.h"
#include <cmath>
#define PLAYER_SPEEDS 10.0f


struct Ray {
    StarVec2D p1, p2;
    StarQuad s1, s2;
    float angle = 0.0f;
    bool movingRight = true;   // <-- NEW: explicit travel direction
    StarLine line;
    bool drawnLine = false;
    float t = 0.0f; 
    bool moving = false;

    // --- Pong feel tuning ---
    float ballSpeed = 4.0f;          // current travel speed, ramps up each rally
    const float baseSpeed = 4.0f;
    const float maxSpeed = 9.0f;
    const float speedRampPerHit = 0.35f;
    const float maxBounceAngle = 65.0f; // steepest angle allowed off a paddle edge

    Ray(StarVec2D startPos) : p1(startPos) {}

    // Classic pong bounce: where the ball hits the paddle (relative to paddle
    // center, -1 at top edge to +1 at bottom edge) determines the return angle.
    // A dead-center hit goes back flat (0 deg); edge hits go back steep.
    static float calculateBounceAngle(float paddleY, float paddleHeight,
                                        float ballY, float ballHeight,
                                        float maxAngle)
    {
        float paddleCenterY = paddleY + (paddleHeight / 2.0f);
        float ballCenterY = ballY + (ballHeight / 2.0f);

        float relative = (ballCenterY - paddleCenterY) / (paddleHeight / 2.0f);
        relative = std::max(-1.0f, std::min(1.0f, relative)); // clamp for edge-clip hits

        return relative * maxAngle;
    }

    void rampUpSpeed()
    {
        ballSpeed = std::min(maxSpeed, ballSpeed + speedRampPerHit);
    }

    float findScreenAdjMagnitude(){
        // distance to whichever wall we're currently heading toward
        if(movingRight)
            return (float)(StariskSettings::WINDOW_WIDTH - 10) - p1.x;
        else
            return p1.x - 10.0f;
    }

    void findScreenEndPoint(){
        float adjMag = findScreenAdjMagnitude();       // always positive magnitude
        float radians = angle * (3.14159265f / 180.0f); 
        float opp = adjMag * tan(radians);
        float dirX = movingRight ? 1.0f : -1.0f;

        p2 = StarVec2D(p1.x + dirX * adjMag, p1.y + opp);
    }

    void drawLine(Starisk* pStar){
        if(drawnLine){
            line.updatePoints(p1, p2, pStar->sbm);
        }
        else{
            line = pStar->CreateLine(p1, p2);
            drawnLine = true;
        }
    }

    void setAngle(float a){ angle = a; }
}; 


void kickOff(std::shared_ptr<Ray> ballRay, Entity& ball);

StariskMain mainProject(GLFWwindow* window, ImGuiContext* ctx)
{

    Starisk* s = new Starisk(window, ctx);
    
    

    Entity& player = s->CreateEntity(50.0f, 100.0f, 30, 80, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SPEEDS);
    player.addComponent<KeyboardMovementComponent>();
    auto& cs = s->addSystem<ColliderSystem>();
    auto& ts = s->addSystem<TransformSystem>();
    auto& kms = s->addSystem<KeyboardMovementSystem>(ctx);
    Entity& ball = s->CreateEntity(StariskSettings::WINDOW_WIDTH/2, StariskSettings::WINDOW_HEIGHT/3, 20, 20, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 0.75f);
    auto ballRay = std::make_shared<Ray>(ball.getComponent<TransformComponent>().pos);
    ballRay->angle = 10.0f;
    Entity& player2 = s->CreateEntity(StariskSettings::WINDOW_WIDTH-50.0f, StariskSettings::WINDOW_HEIGHT/1.7, 30, 80, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), PLAYER_SPEEDS);
    player2.addComponent<KeyboardMovementComponent>();
    //Entity& player3 = s->CreateEntity(200.0f, 20.0f, 30, 80, glm::vec4(0.5f, 0.3f, 0.8f, 1.0f));
    // --- Kickoff: launch the ball toward player 1 or player 2 at game start ---




    KeyboardMovementSystem::setKeyboardMovementComponentCustomLogic(player2.getComponent<KeyboardMovementComponent>(), [&player2, &ball, ballRay](){
        auto& player2Transform = player2.getComponent<TransformComponent>();
        if(Keyboard::key(GLFW_KEY_U)){
            player2Transform.pos.y -= (float)(1 * player2Transform.speed);
        }
        if(Keyboard::key(GLFW_KEY_J)){
            player2Transform.pos.y += (float)(1 * player2Transform.speed);
        }
        if(Keyboard::key(GLFW_KEY_H)){
            player2Transform.pos.x -= (float)(1 * player2Transform.speed);
            
        }
        if(Keyboard::key(GLFW_KEY_K)){
            player2Transform.pos.x += (float)(1 * player2Transform.speed);
        }
        // reset game
        if(Keyboard::key(GLFW_KEY_R)) kickOff(ballRay, ball);

    });

    player.addComponent<ColliderComponent>("AABB");
    ball.addComponent<ColliderComponent>("AABB");
    player2.addComponent<ColliderComponent>("AABB");


    cs.SetAABBSideEffect([s](std::unique_ptr<Entity>& entity, std::unique_ptr<Entity>& other){
        
    });


   kickOff(ballRay, ball);

    cs.SetCustomLogic([s, &ball, ballRay, &player, &player2]() {
        auto& ballTransform = ball.getComponent<TransformComponent>();

        // --- LAUNCH: only runs once per collision, not every frame ---
        if(ColliderSystem::AABB(player, ball) && !ballRay->movingRight){
            auto& playerTransform = player.getComponent<TransformComponent>();
            float playerHeight = player.quad.height;

            float bounceAngle = Ray::calculateBounceAngle(
                playerTransform.pos.y, playerHeight,
                ballTransform.pos.y, ball.quad.height,
                ballRay->maxBounceAngle
            );
            ballRay->setAngle(bounceAngle);
            ballRay->rampUpSpeed();

            ballRay->movingRight = true;  
            ballRay->p1 = ballTransform.pos;
            ballRay->findScreenEndPoint();

            // direction from p1 to p2, normalized, scaled to a speed
            float dx = ballRay->p2.x - ballRay->p1.x;
            float dy = ballRay->p2.y - ballRay->p1.y;
            float len = std::sqrt(dx * dx + dy * dy);

            if(len > 0.0001f){
                float speed = ballRay->ballSpeed;
                ballTransform.velocity = StarVec2D((dx / len) * speed, (dy / len) * speed);
            }

            ballRay->moving = true;
            // ballRay->drawLine(s);
        }


        if(ColliderSystem::AABB(player2, ball) && ballRay->movingRight){
            auto& player2Transform = player2.getComponent<TransformComponent>();
            float player2Height = player2.quad.height;

            float bounceAngle = Ray::calculateBounceAngle(
                player2Transform.pos.y, player2Height,
                ballTransform.pos.y, ball.quad.height,
                ballRay->maxBounceAngle
            );
            ballRay->setAngle(bounceAngle);
            ballRay->rampUpSpeed();

            ballRay->movingRight = false;  
            ballRay->p1 = ballTransform.pos;
            ballRay->findScreenEndPoint();

            // direction from p1 to p2, normalized, scaled to a speed
            float dx = ballRay->p2.x - ballRay->p1.x;
            float dy = ballRay->p2.y - ballRay->p1.y;
            float len = std::sqrt(dx * dx + dy * dy);

            if(len > 0.0001f){
                float speed = ballRay->ballSpeed;
                ballTransform.velocity = StarVec2D((dx / len) * speed, (dy / len) * speed);
            }

            ballRay->moving = true;
            // ballRay->drawLine(s);
        }
        // --- IN-FLIGHT: runs every frame while the ball is traveling ---
        if(ballRay->moving){
            // Bounce off the top/bottom walls: flip vertical velocity and
            // mirror the remaining target point across the wall so the
            // straight-line trajectory continues to reflect correctly.
            float topBound = 10.0f;
            float bottomBound = (float)StariskSettings::WINDOW_HEIGHT - 10.0f - ball.quad.height;

            if(ballTransform.pos.y <= topBound && ballTransform.velocity.y < 0.0f){
                ballTransform.pos.y = topBound;
                ballTransform.velocity.y = -ballTransform.velocity.y;
                ballRay->p2.y = 2.0f * topBound - ballRay->p2.y;
            }
            else if(ballTransform.pos.y >= bottomBound && ballTransform.velocity.y > 0.0f){
                ballTransform.pos.y = bottomBound;
                ballTransform.velocity.y = -ballTransform.velocity.y;
                ballRay->p2.y = 2.0f * bottomBound - ballRay->p2.y;
            }

            float remainingX = ballRay->p2.x - ballTransform.pos.x;
            float remainingY = ballRay->p2.y - ballTransform.pos.y;
            float distRemaining = std::sqrt(remainingX * remainingX + remainingY * remainingY);

            if(distRemaining < 4.0f){ // reached the left/right wall - point scored, relaunch
                ballRay->moving = false;
                kickOff(ballRay, ball);
            }

            // ballRay->drawLine(s);
        }
    });

    return s;
}



void kickOff(std::shared_ptr<Ray> ballRay, Entity& ball){
    auto& ballTransform = ball.getComponent<TransformComponent>();
    ballTransform.pos = StarVec2D(StariskSettings::WINDOW_WIDTH/2, StariskSettings::WINDOW_HEIGHT/3);
    ballRay->ballSpeed = ballRay->baseSpeed; // point scored: reset speed for next rally
    ballRay->movingRight = (std::rand() % 2 == 0); // randomly pick a direction
    ballRay->setAngle(0.0f); // straight shot; or randomize e.g. (rand()%121 - 60) for variety
    ballRay->p1 = ball.getComponent<TransformComponent>().pos;
    ballRay->findScreenEndPoint();

    {
        float dx = ballRay->p2.x - ballRay->p1.x;
        float dy = ballRay->p2.y - ballRay->p1.y;
        float len = std::sqrt(dx * dx + dy * dy);
        if(len > 0.0001f){
            float speed = ballRay->ballSpeed;
            ballTransform.velocity = StarVec2D((dx / len) * speed, (dy / len) * speed);
        }
    }
    ballRay->moving = true;
    // ballRay->drawLine(s);

}