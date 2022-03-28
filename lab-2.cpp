#include <iostream>
#include <string>
#include <random>
#include <vector>
#include <cmath>

constexpr float RADIUS_EQ = 6371008.8;
constexpr float EQ_div_PL = 1.00336409;
constexpr float pi = 3.14159265358979;
constexpr float DEG_TO_RAD = pi/180;

typedef struct coord{
  float sn; //lat
  float we; //lon
}coord;

coord calc_a_b(float ang_A, float way){
  ang_A *=  DEG_TO_RAD;
  float c = way / RADIUS_EQ;
  float a = asin(sin(c) * sin(ang_A));
  float b = acos(cos(c)*cos(a) + sin(c)*sin(a)*cos(ang_A));
  if(ang_A < 0){
    b *= -1;
  }
  coord res;
  res.sn = a / DEG_TO_RAD;
  res.we = b / DEG_TO_RAD;
  return res;
}

float calc_ang_A(coord point, float way){
  float a = point.sn * DEG_TO_RAD;
  float b = point.we * DEG_TO_RAD;
  float c = way / RADIUS_EQ;
  float s = (a + b + c) / 2;
  return 2 * acos(sqrt( (sin(s) * sin(s - a)) / (sin(b) * sin(c)) )) / DEG_TO_RAD; 
}

float calc_dist(coord a1, coord a2){
  float temp1 = sin((a2.sn - a1.sn) * DEG_TO_RAD / 2);
  float temp2 = sin((a2.we - a1.we) * DEG_TO_RAD / 2);
  float a = 2 * RADIUS_EQ * asin(sqrtf(temp1*temp1 + cos(a1.sn * DEG_TO_RAD) * cos(a2.sn * DEG_TO_RAD) * temp2*temp2));
  return a;
}

coord make_coord_normal(coord point){
  while(point.sn > 90.0){
    point.sn -= 90.0;
  }
  while(point.sn < -90.0){
    point.sn += 90.0;
  }
  while(point.we > 180.0){
    point.we -= 180.0;
  }
  while(point.we < -180.0){
    point.we += 180.0;
  }
  return point;
}

class human{
  private:

  std::string fst_nm;
  std::string sec_nm;
  std::string mid_nm;
  int hght;
  float wght;
  std::string hum_inf;
  
  int time_cnt;
  coord loc;
  std::vector<coord> route_history = std::vector<coord>(); 
  

  public:

  human(std::string first_name, std::string second_name, std::string middle_name, unsigned int height_mm, float weight, std::string hum_information, coord location){
    fst_nm = first_name;
    sec_nm = second_name;
    mid_nm = middle_name;
    if(height_mm >= 0){
      hght = height_mm;
    } else{
      hght = 0;
    }
    if(weight >= 0){
      wght = weight;
    } else{
      wght = 0;
    }
    hum_inf = hum_information;
    time_cnt = 0;
    loc = make_coord_normal(location);
    route_history.push_back(loc);
  }

  void makeStep(float ang_of_step, float sz_of_step){
    coord position = calc_a_b(ang_of_step, sz_of_step);
    loc.sn += position.sn;
    loc.we += position.we;
    loc = make_coord_normal(loc);
    route_history.push_back(loc);
    time_cnt++;
  }

  void makeRndStep(){
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1000.0);
    std::uniform_real_distribution<float> direct(0.0, 360.0);
    float sz_of_step = dist(mt);
    float ang_of_step = direct(mt);
    makeStep(ang_of_step, sz_of_step);
  }

  void moveToPlace(coord place){
    loc.sn += place.sn;
    loc.we += place.we;
    loc = make_coord_normal(loc);
    route_history.push_back(loc);
    time_cnt++;
  }

  coord getLocation(){
    return loc;
  }

  float calcDistAt(int time){
    return calc_dist(route_history[time-1], route_history[time]);
  }

  std::string& getFirstName(){
    return fst_nm;
  }

  std::string& getSecondName(){
    return sec_nm;
  }

  std::string& getMiddleName(){
    return mid_nm;
  }

  int getheight_mm(){
    return hght;
  }

  float getWeight(){
    return wght;
  }

  std::string& getInformation(){
    return hum_inf;
  }

  void setFirstName(std::string first_name){
    fst_nm = first_name;
  }

  void setSecondName(std::string second_name){
    sec_nm = second_name;
  }

  void setMiddleName(std::string middle_name){
    mid_nm = middle_name;
  }

  void setheight_mm(int height_mm){
    if (height_mm >= 0){
      hght = height_mm;
    } else{
      hght = 0;
    }
  }

  void setWeight(float weight){
    if (weight >= 0){
      wght = weight;
    } else{
      wght = 0;
    }
  }
  
  void setInformation(std::string hum_information){
    hum_inf = hum_information;
  }
};



int main(int argc, char** argv){
  human Aisaka("Taiga", "Aisaka", "Rikurouvna", 1436, 34, "Palmtop tiger", coord{35.915539, 139.749361});
  for(int i = 1; i < 7; i++){
    Aisaka.makeRndStep();
    std::cout << Aisaka.getLocation().sn << ", " << Aisaka.getLocation().we << std::endl;
  }
  std::cout << Aisaka.calcDistAt(2) << std::endl;
}