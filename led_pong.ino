#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
#include <avr/power.h>
#endif

int goal_size = 5;

int speed_1 = 150;
int speed_1_init_level = 150;
int speed_1_max_level = 20;
int speed_1_reduce = 10;
int pos_1 = 0;
bool pos_1_direction = true;

bool player1_1_pressed = false;
bool player2_1_pressed = false;

// PIN
int led_1_pin = 4;
int player1_1 = 9;
int player2_1 = 10;

// LED
int led_count = 60;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(led_count, led_1_pin, NEO_GRB + NEO_KHZ800);

uint32_t player1_color = pixels.Color(0, 0, 255);
uint32_t player2_color = pixels.Color(255, 0, 0);

uint32_t ball_color = pixels.Color(0, 150, 0);
uint32_t goal_color = pixels.Color(6, 0, 6);
uint32_t goal_color_warn = pixels.Color(255, 0, 0);



void setup() {
  reset_game();

  pixels.begin();
  pinMode(player1_1, INPUT);
  pinMode(player2_1, INPUT);
  Serial.begin(9600);
}

void reset_game()
{
  speed_1 = speed_1_init_level;
  pos_1 = round(led_count / 2);
  pos_1_direction = random_bool();
}

bool random_bool()
{
  return (random(100) >= 50);
}

void speed_up()
{
  if (speed_1 > speed_1_max_level)
    speed_1 = speed_1 - speed_1_reduce;
}

void move_ball()
{
  if (pos_1_direction)
    pos_1++;
  else
    pos_1--;
  delay(speed_1);
}

void print_led_strip()
{
  pixels.clear();
  print_goal();
  pixels.setPixelColor(pos_1, pixels.Color(0, 150, 0));
  pixels.show();
}

void print_goal()
{
  for (int i = 0; i < goal_size; i++)
  {
    pixels.setPixelColor(i, goal_color);
  }

  for (int i = (led_count - 1); i > (led_count - 1 - goal_size); i--)
  {
    pixels.setPixelColor(i, goal_color);
  }
}


void goal_animation(uint32_t c, bool positive_direction) 
{
  for (uint16_t j = 0; j < 10; j++) 
  {
    if (positive_direction)
    {
      for (uint16_t q = 0; q < 4; q++) 
      {
        goal_animation_helper(c, q);
      }
    } else {
      for (uint16_t q = 4; q > 0; q--) 
      {
        goal_animation_helper(c, q);
      }
    }
  }
}

void goal_animation_helper(uint32_t c, uint16_t q)
{
  for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) 
  {
    pixels.setPixelColor(i + q, c);  //turn every third pixel on
  }
  pixels.show();

  delay(50);

  for (uint16_t i = 0; i < pixels.numPixels(); i = i + 3) 
  {
    pixels.setPixelColor(i + q, 0);      //turn every third pixel off
  }
}


bool player1_1_hit()
{
    bool ret = false;
    int current = digitalRead(player1_1);
    if (player1_1_pressed && current == 0)
    {
        player1_1_pressed = false;
    }
    if (!player1_1_pressed && current == 1)
    {
        player1_1_pressed = true;
        ret = true;
    }
    return ret;
}

bool player2_1_hit()
{
    bool ret = false;
    int current = digitalRead(player2_1);
    if (player2_1_pressed && current == 0)
    {
        player2_1_pressed = false;
    }
    if (!player2_1_pressed && current == 1)
    {
        player2_1_pressed = true;
        ret = true;
    }
    return ret;
}

void loop() {
  print_led_strip();

  int player1_hit = player1_1_hit();
  int player2_hit = player2_1_hit();

  if (pos_1 >= (led_count - goal_size) && player1_hit == 1)
  {
    pos_1_direction = false;
    speed_up();
  }
  if (pos_1 >= led_count)
  {
    goal_animation(player2_color, true);
    reset_game();
  }

  if (pos_1 < (goal_size) && player2_hit == 1)
  {
    pos_1_direction = true;
    speed_up();
  }
  if (pos_1 <= 0)
  {
    goal_animation(player1_color, false);
    reset_game();
  }

  move_ball();
}
