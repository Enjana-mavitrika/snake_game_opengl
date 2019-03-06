/*********************************************************************
 *                                                                    
 *                                                                   
 *  PROJET : SPACE SNAKE 
 *
 *  Auteur : S.RABONARIJAONA
 *
 *  Fonctionnalités:
 *     draw_dragon()--------------------------OK
 *     avancer_dragon()-----------------------OK
 *     tourner_dragon_haut()------------------OK
 *     tourner_dragon_bas()-------------------OK
 *     tourner_dragon_gauche()----------------OK
 *     tourner_dragon_droite()----------------OK
 *     dessiner_sol()-------------------------OK
 *     dessiner_obstacle_aleatoire()----------OK
 *     gerer_colision_obstacle_serpent()------OK
 *     dessiner_bonus_aleatoire()-------------OK
 *     gerer_colision_bonus_serpent()---------OK
 *     animer_obstacle()----------------------OK
 *     rajouter_sol_niveau_2------------------OK
 *     gerer_obstacle_niveau_2----------------OK
 *     gerer_bonus_niveau_2-------------------OK
 *     gerer_passage_niveau_2_vers_niveau_1---STBY
 *     rajouter_sol_niveau_3------------------STBY
 *     gerer_obstacle_niveau_3----------------STBY
 *     gerer_bonus_niveau_3-------------------STBY
 *     gerer_passage_niveau_3_vers_niveau_2---STBY
 *     gerer_joueur + score-------------------STBY
 *     
 *
 ************************************************************************/





#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <math.h>
#include <string.h>
#include <time.h>

/* biblio OpenGL */
#include "GL/gl.h"
#include "GL/glut.h"


#define TAILLE_MAX 20
#define PI 3.14

void changeSize(int w, int h) {

  // Prevent a divide by zero, when window is too short
  // (you cant make a window of zero width).
  if (h == 0)
    h = 1;

  float ratio =  w * 1.0 / h;

  // Use the Projection Matrix
  glMatrixMode(GL_PROJECTION);

  // Reset Matrix
  glLoadIdentity();

  // Set the viewport to be the entire window
  glViewport(0, 0, w, h);

  // Set the correct perspective.
  gluPerspective(45.0f, ratio, 0.1f, 700.0f);

  // Get Back to the Modelview
  glMatrixMode(GL_MODELVIEW);
}




// type Boule represente partie corp dragon
typedef struct{
  float centre_x;
  float centre_y;
  float centre_z;
  float rayon;
} Boule;


// variables globaux
int taille_dragon = 8;
Boule dragon[TAILLE_MAX];
Boule dragon_derniere_pos[TAILLE_MAX];
float angle_rotation = 0.0f;
int avancer_flag = 0;
enum direction{
  DROITE,
  GAUCHE,
  HAUT,
  BAS,
  AVANT,
  ARRIERE,
  MONTER,
};

enum direction direction_flag = BAS;

void copier_dragon(const Boule d_src[TAILLE_MAX], Boule d_dest[TAILLE_MAX]){
  int i;
  for (i = 0; i <= taille_dragon; i++){
    d_dest[i].centre_x = d_src[i].centre_x;
    d_dest[i].centre_y = d_src[i].centre_y;
    d_dest[i].centre_z = d_src[i].centre_z;
  }
}

void save_dragon_derniere_pos(){
  copier_dragon(dragon, dragon_derniere_pos);
}

float debut_serpent = -500.0f * 5.0f;

void init_dragon(Boule d[TAILLE_MAX], int taille){
  int i;
  for (i=0; i<taille; i++){
    d[i].centre_x = 0.0f;
    d[i].centre_y = 2.0f;
    d[i].centre_z = debut_serpent;
    d[i].rayon = 2.0f;
    debut_serpent += 6.0f;
  }
  save_dragon_derniere_pos();
}

void move_dragon(Boule d[TAILLE_MAX], int partie){
  glTranslatef(d[partie].centre_x, d[partie].centre_y, d[partie].centre_z);
}

void draw_dragon(Boule d[TAILLE_MAX], int taille){
  int i;
  for (i = 0; i < taille; i++){
    glColor3f(i/10.0f,i/10.0, i/10.0f);
    glPushMatrix();
    move_dragon(d, i);
    glutSolidSphere(d[i].rayon, 20, 20);
    //glutSolidCube(1.0d);
    glPopMatrix();
    
  }
}

void avancer_dragon(){
  // bouger dragon selon position de sa tete
  int i;
  switch(direction_flag){
  case MONTER:
    dragon[0].centre_z -= 4.0f;
    dragon[0].centre_y += 0.8f;
    break;
  case DROITE:
    dragon[0].centre_x += 1.0f;
    break;
  case GAUCHE:
    dragon[0].centre_x -= 1.0f;
    break;
  case ARRIERE:
    dragon[0].centre_z += 1.0f;
    break;
  case AVANT:
    dragon[0].centre_z -= 6.0f;
    break;
  case HAUT:
    dragon[0].centre_y += 1.0f;
    break;
  case BAS:
    dragon[0].centre_y -= 6.0f;
    break;
  }
  for (i = 1; i <= taille_dragon; i++){
    dragon[i].centre_x = dragon_derniere_pos[i-1].centre_x;
    dragon[i].centre_y = dragon_derniere_pos[i-1].centre_y;
    dragon[i].centre_z = dragon_derniere_pos[i-1].centre_z;
  }
  save_dragon_derniere_pos();
  glutPostRedisplay();
}

void tourner_dragon_haut(){
  dragon[0].centre_y--;
  if (direction_flag == AVANT){
    direction_flag = HAUT;
  }
  else if (direction_flag == BAS){
    direction_flag = AVANT;
  }
  else if (direction_flag == HAUT){
    direction_flag = ARRIERE;
  }
  else if (direction_flag == ARRIERE){
    direction_flag = BAS;
  }
  else if (direction_flag == DROITE){
    direction_flag = HAUT;
  }
  else if (direction_flag == GAUCHE){
    direction_flag = BAS;
  }
  dragon[0].centre_y += 1.0f;
}

void tourner_dragon_bas(){
  dragon[0].centre_y++;
  if (direction_flag == AVANT){
    direction_flag = BAS;
  }
  else if (direction_flag == HAUT){
    direction_flag = AVANT;
  }
  else if (direction_flag == BAS){
    direction_flag = ARRIERE;
  }
  else if (direction_flag == ARRIERE){
    direction_flag = HAUT;
  }
  else if (direction_flag == DROITE){
    direction_flag = BAS;
  }
  else if (direction_flag == GAUCHE){
    direction_flag = BAS;
  }
  dragon[0].centre_y -= 1.0f;
}

void tourner_dragon_droite(){
  /* bouger le dragon comme snake classique */
  /* dragon[0].centre_x--; */
  /* if (direction_flag == AVANT){ */
  /*   direction_flag = DROITE; */
  /* } */
  /* else if (direction_flag == GAUCHE){ */
  /*   direction_flag = AVANT; */
  /* } */
  /* else if (direction_flag == DROITE){ */
  /*   direction_flag = ARRIERE; */
  /* } */
  /* else if (direction_flag == ARRIERE){ */
  /*   direction_flag = GAUCHE; */
  /* } */
  /* else if (direction_flag == HAUT){ */
  /*   direction_flag = DROITE; */
  /* } */
  /* else if (direction_flag == BAS){ */
  /*   direction_flag = DROITE; */
  /* } */
  dragon[0].centre_x += 2.0f;
}

void tourner_dragon_gauche(){
  /* bouger le dragon comme snake classique */
  //dragon[0].centre_x++;
  /* if (direction_flag == AVANT){ */
  /*   direction_flag = GAUCHE; */
  /* } */
  /* else if (direction_flag == DROITE){ */
  /*   direction_flag = AVANT; */
  /* } */
  /* else if (direction_flag == GAUCHE){ */
  /*   direction_flag = ARRIERE; */
  /* } */
  /* else if (direction_flag == ARRIERE){ */
  /*   direction_flag = DROITE; */
  /* } */
  /* else if (direction_flag == HAUT){ */
  /*   direction_flag = DROITE; */
  /* } */
  /* else if (direction_flag == BAS){ */
  /*   direction_flag = DROITE; */
  /* } */
  dragon[0].centre_x -= 2.0f;
}




/********************************


          FONCTIONS UTILES


*********************************/
float rand_a_b(int a, int b){     // retourne rand entre [a,b]
  return (rand()%(b-a) + a)*1.0f;
}




/****************************


          OBSTACLES


******************************/



enum type_sol{
  NORMAL,
  MILIEU,
  ALIGN_DROITE,
  ALIGN_GAUCHE
};

enum type_sol sol_niveau_1[30] = {NORMAL, ALIGN_DROITE, MILIEU, ALIGN_GAUCHE, NORMAL, MILIEU, NORMAL, ALIGN_GAUCHE, MILIEU, NORMAL, ALIGN_GAUCHE, MILIEU, ALIGN_GAUCHE, NORMAL, ALIGN_DROITE, MILIEU, ALIGN_GAUCHE, NORMAL, MILIEU, NORMAL,/*20*/MILIEU,ALIGN_DROITE,ALIGN_DROITE,NORMAL,MILIEU,NORMAL,MILIEU,ALIGN_GAUCHE,ALIGN_GAUCHE,NORMAL/*30*/};
enum type_sol sol_niveau_2[30] = {NORMAL, ALIGN_DROITE, MILIEU, ALIGN_GAUCHE, NORMAL, MILIEU, NORMAL, ALIGN_GAUCHE, MILIEU, NORMAL, ALIGN_GAUCHE, MILIEU, ALIGN_GAUCHE, NORMAL, ALIGN_DROITE, MILIEU, ALIGN_GAUCHE, NORMAL, MILIEU, NORMAL,/*20*/MILIEU,ALIGN_DROITE,ALIGN_DROITE,NORMAL,MILIEU,NORMAL,MILIEU,ALIGN_GAUCHE,ALIGN_GAUCHE,NORMAL/*30*/};




typedef struct{
  int taille;
  int direction_deplacement;
  float centre_x;
  float centre_y;
  float centre_z;
}
  Obstacle;

Obstacle list_obstacle[1000];
int compteur_obstacle_flag = 0;
int nbr_obstacle = 0;

Obstacle list_obstacle_niv_2[1000];
int compteur_obstacle_niv_2_flag = 0;
int nbr_obstacle_niv_2 = 0;

void desactiver_compteur_obstacle(int niveau){
  switch(niveau){
  case 1:
    compteur_obstacle_flag = 1;
    break;
  case 2:
    compteur_obstacle_niv_2_flag = 1;
  }
}

void init_obstacle(Obstacle list[1000], int niveau, int indice, int taille, float posX, float posY, float posZ){
  list[indice].direction_deplacement = 1;
  list[indice].taille = taille;
  list[indice].centre_x = posX;
  list[indice].centre_y = posY;
  list[indice].centre_z = posZ;
  switch(niveau){
  case 1:
    if (!compteur_obstacle_flag)
      nbr_obstacle++;
    break;
  case 2:
    if (!compteur_obstacle_niv_2_flag)
      nbr_obstacle_niv_2++;
    break;
  }
}


void deplacer_obstacle(Obstacle list[1000], int indice){
   glTranslatef(list[indice].centre_x, list[indice].centre_y, list[indice].centre_z);
}

void dessiner_obstacle(Obstacle list[1000], int niveau){
  int i;
  int taille_list;
  switch(niveau){
  case 1:
    taille_list = nbr_obstacle;
    break;
  case 2:
    taille_list = nbr_obstacle_niv_2;
    break;
  }
  for (i=0; i<taille_list; i++){
    glColor3f(1.0f, 0.0f, 0.0f); // rouge
    glPushMatrix();
    deplacer_obstacle(list, i);
    glutSolidCube(list[i].taille);
    glPopMatrix();
  }
}


void init_obstacle_alea(int nbr_obstacle_par_sol){
  int i,j,k=0;
  float limit_gauche, limit_droite;
  float limit_avant = -500.0f, limit_arriere = 30.0f;
  float x, z;
  for (i=0; i<30; i++){ // sur chaque sol
    switch(sol_niveau_1[i]){
    case NORMAL:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 30.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -30.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -15.0f; limit_droite = 15.0f;
      break;
    }
 
    for (j=0; j < nbr_obstacle_par_sol; j++){
      x = rand_a_b(limit_gauche, limit_droite);
      z = rand_a_b(limit_avant, limit_arriere);
      init_obstacle(list_obstacle, 1, k, 10, x, 5.0f, z);
      k++;
    }
    limit_avant = -500*(i+2); limit_arriere = -500*(i+1);
  }
}


// gestion obstacle niveau 2

void init_obstacle_niv_2_alea(int nbr_obstacle_par_sol){
  int i,j,k=0;
  float limit_gauche, limit_droite;
  float limit_avant = -500.0f, limit_arriere = 30.0f;
  float x, z;
  for (i=0; i<30; i++){ // sur chaque sol
    switch(sol_niveau_2[i]){
    case NORMAL:
      limit_gauche = -60.0f; limit_droite = 60.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 60.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -60.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    }
 
    for (j=0; j < nbr_obstacle_par_sol; j++){
      x = rand_a_b(limit_gauche, limit_droite);
      z = rand_a_b(limit_avant, limit_arriere);
      init_obstacle(list_obstacle_niv_2, 2, k, 10, x, -45.0f, z);
      k++;
    }
    limit_avant = -500*(i+2); limit_arriere = -500*(i+1);
  }
}







void animer_obstacle(int indice){
  int position_sol = (int)(list_obstacle[indice].centre_z*-1)/500;
  float limit_gauche; float limit_droite;
  switch(sol_niveau_1[position_sol]){
    case NORMAL:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 30.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -30.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -15.0f; limit_droite = 15.0f;
      break;
    }
  if (list_obstacle[indice].centre_x >= limit_droite)
    list_obstacle[indice].direction_deplacement = -1;
  else if (list_obstacle[indice].centre_x <= limit_gauche){
    list_obstacle[indice].direction_deplacement = 1;
  }
  list_obstacle[indice].centre_x += 1*list_obstacle[indice].direction_deplacement;
}


 

int detecter_colision_obstacle(const Obstacle ob1){
  if (((dragon[0].centre_x - dragon[0].rayon >= ob1.centre_x - ob1.taille/2 && dragon[0].centre_x - dragon[0].rayon <= ob1.centre_x + ob1.taille/2) || (dragon[0].centre_x + dragon[0].rayon >= ob1.centre_x - ob1.taille/2 && dragon[0].centre_x + dragon[0].rayon <= ob1.centre_x + ob1.taille/2)) && dragon[0].centre_z - dragon[0].rayon >= ob1.centre_z - ob1.taille/2 && dragon[0].centre_z - dragon[0].rayon <= ob1.centre_z + ob1.taille/2){
    return 1;
  }
  return 0;
}

int detecter_colision_obstacles(){
  int i;
  if (dragon[0].centre_y == 2.0f){
    for (i=0; i < nbr_obstacle; i++){
      if (detecter_colision_obstacle(list_obstacle[i]))
	return 1;
    }
  }else if (dragon[0].centre_y == -47.0f){
    for (i=0; i < nbr_obstacle_niv_2; i++){
      if (detecter_colision_obstacle(list_obstacle_niv_2[i]))
	return 1;
    }
  }
  return 0;
}



// fonction dessiner les sols niveau 1
void dessiner_sol_normal_niv1(float limit_avant, float limit_arriere){
  glColor3f(0.78f, 0.082f, 0.52f);
  glBegin(GL_QUADS);
  glVertex3f(-30.0f, 0.0f, limit_avant); // gauche avant
  glVertex3f(-30.0f, 0.0f, limit_arriere);   // gauche arriere
  glVertex3f(30.0f, 0.0f, limit_arriere);    // droite arriere
  glVertex3f(30.0f, 0.0f, limit_avant);  // droite avant
  glEnd();
}

void dessiner_sol_droite_niv1(float limit_avant, float limit_arriere){
  glColor3f(0.78f, 0.082f, 0.52f);
  glBegin(GL_QUADS);
  glVertex3f(0.0f, 0.0f, limit_avant);
  glVertex3f(0.0f, 0.0f, limit_arriere);
  glVertex3f(30.0f, 0.0f, limit_arriere);
  glVertex3f(30.0f, 0.0f, limit_avant);
  glEnd();
}

void dessiner_sol_gauche_niv1(float limit_avant, float limit_arriere){
  glColor3f(0.78f, 0.082f, 0.52f);
  glBegin(GL_QUADS);
  glVertex3f(-30.0f, 0.0f, limit_avant);
  glVertex3f(-30.0f, 0.0f, limit_arriere);
  glVertex3f(0.0f, 0.0f, limit_arriere);
  glVertex3f(0.0f, 0.0f, limit_avant);
  glEnd();
}

void dessiner_sol_milieu_niv1(float limit_avant, float limit_arriere){
  glColor3f(0.78f, 0.082f, 0.52f);
  glBegin(GL_QUADS);
  glVertex3f(-15.0f, 0.0f, limit_avant);
  glVertex3f(-15.0f, 0.0f, limit_arriere);
  glVertex3f(15.0f, 0.0f, limit_arriere);
  glVertex3f(15.0f, 0.0f, limit_avant);
  glEnd();
}


// fonction dessiner les sols niveau 2

void dessiner_sol_normal_niv2(float limit_avant, float limit_arriere){
  glColor3f(0.06f, 0.20f, 0.65f);
  glBegin(GL_QUADS);
  glVertex3f(-60.0f, -50.0f, limit_avant); // gauche avant
  glVertex3f(-60.0f, -50.0f, limit_arriere);   // gauche arriere
  glVertex3f(60.0f, -50.0f, limit_arriere);    // droite arriere
  glVertex3f(60.0f, -50.0f, limit_avant);  // droite avant
  glEnd();
}

void dessiner_sol_droite_niv2(float limit_avant, float limit_arriere){
  glColor3f(0.06f, 0.20f, 0.65f);
  glBegin(GL_QUADS);
  glVertex3f(0.0f, -50.0f, limit_avant);
  glVertex3f(0.0f, -50.0f, limit_arriere);
  glVertex3f(60.0f, -50.0f, limit_arriere);
  glVertex3f(60.0f, -50.0f, limit_avant);
  glEnd();
}

void dessiner_sol_gauche_niv2(float limit_avant, float limit_arriere){
  glColor3f(0.06f, 0.20f, 0.65f);
  glBegin(GL_QUADS);
  glVertex3f(-60.0f, -50.0f, limit_avant);
  glVertex3f(-60.0f, -50.0f, limit_arriere);
  glVertex3f(0.0f, -50.0f, limit_arriere);
  glVertex3f(0.0f, -50.0f, limit_avant);
  glEnd();
}

void dessiner_sol_milieu_niv2(float limit_avant, float limit_arriere){
  glColor3f(0.06f, 0.20f, 0.65f);
  glBegin(GL_QUADS);
  glVertex3f(-30.0f, -50.0f, limit_avant);
  glVertex3f(-30.0f, -50.0f, limit_arriere);
  glVertex3f(30.0f, -50.0f, limit_arriere);
  glVertex3f(30.0f, -50.0f, limit_avant);
  glEnd();
}


// passages entre niveau

// dessiner passage entre niveau 1 et 2

typedef struct{
  float limit_avant;
  float limit_arriere;
  float limit_gauche;
  float limit_droite;
}
  Passage;

Passage passages[20];
int nbr_passages = 0;

void init_passage_gauche(float lim_av, float lim_ar){
  passages[nbr_passages].limit_avant = lim_av;
  passages[nbr_passages].limit_arriere = lim_ar;
  passages[nbr_passages].limit_gauche = -30.0f;
  passages[nbr_passages].limit_droite = -15.0f;
  nbr_passages++;
}

void init_passage_droite(float lim_av, float lim_ar){
  passages[nbr_passages].limit_avant = lim_av;
  passages[nbr_passages].limit_arriere = lim_ar;
  passages[nbr_passages].limit_gauche = 15.0f;
  passages[nbr_passages].limit_droite = 30.0f;
  nbr_passages++;
}


void dessiner_passages(){
  int i;
  for(i=0; i<nbr_passages; i++){
    glColor3f(0.42f, 0.0f, 0.46f); // violet
    glBegin(GL_QUADS);
    glVertex3f(passages[i].limit_gauche, 0.0f, passages[i].limit_avant);
    glVertex3f(passages[i].limit_gauche, -50.0f, passages[i].limit_arriere);
    glVertex3f(passages[i].limit_droite, -50.0f, passages[i].limit_arriere);
    glVertex3f(passages[i].limit_droite, 0.0f, passages[i].limit_avant);
    glEnd();
  }
}


// gerer monter du serpent sur les passages entre niveau

int detecter_passages(){
  int i;
  float j, k;
  for (i=0; i<nbr_passages; i++){
    if (dragon[0].centre_x >= passages[i].limit_gauche && dragon[0].centre_x <= passages[i].limit_droite && dragon[0].centre_z <= passages[i].limit_arriere && dragon[0].centre_z >= passages[i].limit_avant){
    // restabiliser le serpent
      if (dragon[0].centre_y > 2.0f){
	dragon[0].centre_z -= 6.0f;
	dragon[0].centre_y = 2.0f;
	direction_flag = AVANT;
      }
      if (dragon[0].centre_y <-47.0f){
      	dragon[0].centre_y = -47.0f;
	direction_flag = AVANT;
      }
    }
    for (j=passages[i].limit_arriere, k=-47.0f; j>=passages[i].limit_avant && k < 2.0f; j-=4.0f, k+=0.8f){
      if (dragon[0].centre_z <= j && dragon[0].centre_z >= j - 4.0f && dragon[0].centre_y >= k && dragon[0].centre_y <= k + 0.8f && dragon[0].centre_x >= passages[i].limit_gauche && dragon[0].centre_x <= passages[i].limit_droite){
	dragon[0].centre_y = k;
	return 1;
      }
    }
  }

  return 0;
}







// retourne 1 si detecte chute sinon 0
int detecter_chute(){
  int i;
  float limit_gauche, limit_droite;
  i = (int)((dragon[0].centre_z*-1) / 500);
  
  if (dragon[0].centre_y >= 2.0f-2.0f && dragon[0].centre_y <= 2.0f+2.0f){
    dragon[0].centre_y = 2.0f;
    switch(sol_niveau_1[i]){
    case NORMAL:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 30.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -30.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -15.0f; limit_droite = 15.0f;
      break;
    }
  }
  else if (dragon[0].centre_y <= -47.0f+2.0f && dragon[0].centre_y >= -47.0f-2.0f){
    dragon[0].centre_y = -47.0f;
    switch(sol_niveau_2[i]){
    case NORMAL:
      limit_gauche = -60.0f; limit_droite = 60.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 60.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -60.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    }
  }
  
  if (dragon[0].centre_x >= limit_gauche && dragon[0].centre_x <= limit_droite ){
    return 0;
  }
  // chute 
  return 1;
}









typedef struct{
  int gagner;
  int rayon;
  float centre_x;
  float centre_y;
  float centre_z;
}
  Bonus;

int bonus_joueur = 0;

Bonus list_bonus[1000];
int nbr_bonus = 0;
int compteur_bonus_flag = 0;

Bonus list_bonus_niv_2[1000];
int nbr_bonus_niv_2 = 0;
int compteur_bonus_niv_2_flag = 0;

void desactiver_compteur_bonus(int niveau){
  switch(niveau){
  case 1: compteur_bonus_flag = 1; break;
  case 2: compteur_bonus_niv_2_flag = 1; break;
  }
}


void afficher_bonus(){
  int i;
  for (i=0; i<nbr_bonus; i++){
    printf("init_bonus(%d, 5, %.1f, %.1f, %.1f);\n", i, list_bonus[i].centre_x, list_bonus[i].centre_y, list_bonus[i].centre_z);
  }
}


void init_bonus(Bonus list[1000], int niveau, int indice, int rayon, float posX, float posY, float posZ){
  list[indice].gagner = 0;
  list[indice].rayon = rayon;
  list[indice].centre_x = posX;
  list[indice].centre_y = posY;
  list[indice].centre_z = posZ;
  switch(niveau){
  case 1:
    if (!compteur_bonus_flag)
      nbr_bonus++;
    break;
  case 2:
    if (!compteur_bonus_niv_2_flag)
      nbr_bonus_niv_2++;
    break;
  }
}

void deplacer_bonus(Bonus list[1000], int indice){
  glTranslatef(list[indice].centre_x, list[indice].centre_y, list[indice].centre_z);
}

// dessiner bonus
void dessiner_bonus(Bonus list[1000], int niveau){
  int i;
  int taille_list;
  switch(niveau){
  case 1: taille_list = nbr_bonus; break;
  case 2: taille_list = nbr_bonus_niv_2; break;
  }
  for (i=0; i<taille_list; i++){
    if (!list[i].gagner){
      glColor3f(1.0f, 0.27f, 0.0f);  // orange
      glPushMatrix();
      deplacer_bonus(list, i);
      glutWireSphere(list[i].rayon, 20, 20);
      glPopMatrix();
    }
  }
}


// init score alea

void init_bonus_alea(int nbr_bonus_par_sol){
  int i,j,k=0;
  float limit_gauche, limit_droite;
  float limit_avant = -500.0f, limit_arriere = 30.0f;
  float x, z;
  for (i=0; i<30; i++){ // sur chaque sol
    switch(sol_niveau_1[i]){
    case NORMAL:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 30.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -30.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -15.0f; limit_droite = 15.0f;
      break;
    }
 
    for (j=0; j < nbr_bonus_par_sol; j++){ 
      x = rand_a_b(limit_gauche, limit_droite);
      z = rand_a_b(limit_avant, limit_arriere);
      init_bonus(list_bonus, 1, k, 5, x, 5.0f, z);
      k++;
    }
    limit_avant = -500*(i+2); limit_arriere = -500*(i+1);
  }
}

void init_bonus_niv_2_alea(int nbr_bonus_par_sol){
  int i,j,k=0;
  float limit_gauche, limit_droite;
  float limit_avant = -500.0f, limit_arriere = 30.0f;
  float x, z;
  for (i=0; i<30; i++){ // sur chaque sol
     switch(sol_niveau_2[i]){
    case NORMAL:
      limit_gauche = -60.0f; limit_droite = 60.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 60.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -60.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    }
 
    for (j=0; j < nbr_bonus_par_sol; j++){ 
      x = rand_a_b(limit_gauche, limit_droite);
      z = rand_a_b(limit_avant, limit_arriere);
      init_bonus(list_bonus_niv_2, 2, k, 5, x, -45.0f, z);
      k++;
    }
    limit_avant = -500*(i+2); limit_arriere = -500*(i+1);
  }
}


int detecter_colision_bonus (const Bonus bonus){
  if (((dragon[0].centre_x - dragon[0].rayon >= bonus.centre_x - bonus.rayon && dragon[0].centre_x - dragon[0].rayon <= bonus.centre_x + bonus.rayon) || (dragon[0].centre_x + dragon[0].rayon >= bonus.centre_x - bonus.rayon && dragon[0].centre_x + dragon[0].rayon <= bonus.centre_x + bonus.rayon)) && dragon[0].centre_z - dragon[0].rayon >= bonus.centre_z - bonus.rayon && dragon[0].centre_z - dragon[0].rayon <= bonus.centre_z + bonus.rayon){
    return 1;
  }
  return 0;
}

int detecter_colision_list_bonus(){
  int i;
  if (dragon[0].centre_y == 2.0f){
    for (i=0; i < nbr_bonus; i++){
      if (detecter_colision_bonus(list_bonus[i])){
	list_bonus[i].gagner = 1;
	return 1;
      }
    }
  }else if (dragon[0].centre_y == -47.0f){
    for (i=0; i < nbr_bonus_niv_2; i++){
      if (detecter_colision_bonus(list_bonus_niv_2[i])){
	list_bonus_niv_2[i].gagner = 1;
	return 1;
      }
    }
  }
  return 0;
}



// bonus special vitesse

Bonus list_bonus_vitesse[1000];
int nbr_bonus_vitesse = 0;
int bonus_vitesse_joueur = 0;
int compteur_bonus_vitesse_flag = 0;

void desactiver_compteur_bonus_vitesse(){
  compteur_bonus_vitesse_flag = 1;
}

void init_bonus_vitesse(int indice, int rayon, float posX, float posY, float posZ){
  list_bonus_vitesse[indice].gagner = 0;
  list_bonus_vitesse[indice].rayon = rayon;
  list_bonus_vitesse[indice].centre_x = posX;
  list_bonus_vitesse[indice].centre_y = posY;
  list_bonus_vitesse[indice].centre_z = posZ;
  if (!compteur_bonus_vitesse_flag)
    nbr_bonus_vitesse++;
}

void deplacer_bonus_vitesse(int indice){
  glTranslatef(list_bonus_vitesse[indice].centre_x, list_bonus_vitesse[indice].centre_y, list_bonus_vitesse[indice].centre_z);
}

void dessiner_bonus_vitesse(){
  int i;
  for (i=0; i<nbr_bonus_vitesse; i++){
    if (!list_bonus_vitesse[i].gagner){
      glColor3f(0.29f, 0.0f, 0.50f);  // indigo
      glPushMatrix();
      deplacer_bonus_vitesse(i);
      glutWireTorus(list_bonus_vitesse[i].rayon/4.0d, list_bonus_vitesse[i].rayon, 20, 20);
      //glutWireSphere(list_bonus_vitesse[i].rayon, 20, 20);
      glPopMatrix();
    }
  }
}

void init_bonus_vitesse_alea(int nbr_bonus_par_sol){
  int i,j,k=0;
  float limit_gauche, limit_droite;
  float limit_avant = -500.0f, limit_arriere = 30.0f;
  float x, z;
  for (i=0; i<30; i++){ // sur chaque sol
    switch(sol_niveau_1[i]){
    case NORMAL:
      limit_gauche = -30.0f; limit_droite = 30.0f;
      break;
    case ALIGN_DROITE:
      limit_gauche = 0.0f; limit_droite = 30.0f;
      break;
    case ALIGN_GAUCHE:
      limit_gauche = -30.0f; limit_droite = 0.0f;
      break;
    case MILIEU:
      limit_gauche = -15.0f; limit_droite = 15.0f;
      break;
    }
 
    for (j=0; j < nbr_bonus_par_sol; j++){ 
      x = rand_a_b(limit_gauche, limit_droite);
      z = rand_a_b(limit_avant, limit_arriere);
      init_bonus_vitesse(k, 5, x, 5.0f, z);
      k++;
    }
    limit_avant = -500*(i+2); limit_arriere = -500*(i+1);
  }
}


int detecter_colision_list_bonus_vitesse(){
  int i;
  for (i=0; i < nbr_bonus; i++){
    if (dragon[0].centre_y == 2.0f && detecter_colision_bonus(list_bonus_vitesse[i])){
      list_bonus_vitesse[i].gagner = 1;
      return 1;
    }
  }
  return 0;
}




// Ecris sur l'ecran
void ecrire( int x, int y, int z, char *st)
{
  int l,i;
  l=strlen( st ); 
  glRasterPos3i( x, y, z); 
  for( i=0; i < l; i++) 
    {
      glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, st[i]);
    }
}

void ecrire_point_vie(){
  //int w = glutGet(GLUT_WINDOW_WIDTH);
  //int h = glutGet(GLUT_WINDOW_HEIGHT);
  int x,y,z;
  char string_point_vie[10];
  char message[50] = "POINT DE VIE : ";
  //char infos [50];
  sprintf(string_point_vie, "%d", taille_dragon);
  strcat (message, string_point_vie);
  x = dragon[0].centre_x+30.0f - strlen(message);
  y = dragon[0].centre_y + 30;
  z = dragon[0].centre_z;
  glColor3f(0.0,0.0,1.0);
  ecrire(x, y, z, message);
}

void ecrire_point_bonus(){
  int x,y,z;
  char string_point_vie[10];
  char message[50] = "BONUS : ";
  //char infos [50];
  sprintf(string_point_vie, "%d", bonus_joueur);
  strcat (message, string_point_vie);
  x = dragon[0].centre_x+30.0f - strlen(message);
  y = dragon[0].centre_y + 35;
  z = dragon[0].centre_z;
  glColor3f(1.0,0.0,0.0);
  ecrire(x, y, z, message);
}



void animer(void){
  if (dragon[0].centre_y < -100.0f){
    debut_serpent = dragon[0].centre_z + 200;
    taille_dragon--;
    bonus_joueur -= 5;
    init_dragon(dragon, taille_dragon);
    direction_flag = AVANT;
  }
  if (detecter_colision_list_bonus()){
    bonus_joueur++;
  }
  if (detecter_colision_list_bonus_vitesse()){
    bonus_vitesse_joueur++;
  }
  if (!detecter_passages()){
    if (!detecter_chute() && ((dragon[0].centre_y == -47.0f) || dragon[0].centre_y == 2.0f)){
      direction_flag = AVANT;
    }
    else{
      printf("CHUTE !!\n");
      direction_flag = BAS;
    }
  }else{
    direction_flag = MONTER;
  }
  if (!detecter_colision_obstacles()){
    avancer_dragon();
  }else{
    bonus_joueur -= 5;
    debut_serpent = dragon[0].centre_z + 200;
    taille_dragon--;
    init_dragon(dragon, taille_dragon);
    direction_flag = AVANT;
  }
  usleep(80000);
}



void renderScene(void) {

  // Clear Color and Depth Buffers
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  /* for (int i = 0; i<taille_dragon; i++) */
  /*   printf("%f %f %f\n", dragon[i].centre_x, dragon[i].centre_y, dragon[i].centre_z); */

  // Mettre à jour transformation
  glLoadIdentity();
  // Positionner la caméra
  if (/* direction_flag == MONTER ||  */direction_flag == AVANT || direction_flag == HAUT /* || direction_flag == BAS */){
    gluLookAt( dragon[0].centre_x, dragon[0].centre_y + 20.0f, dragon[0].centre_z + 70.0f,
	       dragon[0].centre_x, dragon[0].centre_y,  dragon[0].centre_z-60.0f,
	       0.0f, 1.0f,  0.0f);
  }
  else if (direction_flag == BAS){
    gluLookAt(
	      dragon[0].centre_x, dragon[0].centre_y + 70.0f, dragon[0].centre_z+5.0f,
	       dragon[0].centre_x, dragon[0].centre_y-30.0f,  dragon[0].centre_z,
	       0.0f, 1.0f,  0.0f);
  }
  else if (direction_flag == MONTER){
    gluLookAt( dragon[0].centre_x, dragon[0].centre_y + 5.0f, dragon[0].centre_z + 50.0f,
	       dragon[0].centre_x, dragon[0].centre_y,  dragon[0].centre_z-60.0f,
	       0.0f, 1.0f,  0.0f);
  }
  else if (direction_flag == ARRIERE){
    gluLookAt( dragon[taille_dragon-1].centre_x + 20.0f, dragon[taille_dragon-1].centre_y + 50.0f, dragon[taille_dragon-1].centre_z - 30.0f,
	       dragon[0].centre_x, dragon[0].centre_y,  dragon[0].centre_z,
	       0.0f, 1.0f,  0.0f);
  }
  else if (direction_flag == DROITE){
    gluLookAt( dragon[0].centre_x - 40.0f, dragon[0].centre_y + 50.0f, dragon[0].centre_z,
	       dragon[0].centre_x, dragon[0].centre_y,  dragon[0].centre_z,
	       0.0f, 1.0f,  0.0f);
  }
  else if (direction_flag == GAUCHE){
    gluLookAt( dragon[0].centre_x + 40.0f, dragon[0].centre_y + 50.0f, dragon[0].centre_z,
	       dragon[0].centre_x, dragon[0].centre_y,  dragon[0].centre_z,
	       0.0f, 1.0f,  0.0f);
  }

  ecrire_point_vie();
  ecrire_point_bonus();

  draw_dragon(dragon, taille_dragon);
  

  // dessiner passages
  dessiner_sol_normal_niv1(-500.0f, 30.0f);
  dessiner_sol_droite_niv1(-500.0f*2, -500.0f);
  dessiner_sol_milieu_niv1(-500.0f*3, -500.0f*2);
  dessiner_sol_gauche_niv1(-500.0f*4, -500.0f*3);
  dessiner_sol_normal_niv1(-500.0f*5, -500.0f*4);
  dessiner_sol_milieu_niv1(-500.0f*6, -500.0f*5);
  //init_passage_gauche(-500.0f*6, -500.0f*6 +250.0f);
  dessiner_sol_normal_niv1(-500.0f*7, -500.0f*6);
  dessiner_sol_gauche_niv1(-500.0f*8, -500.0f*7);
  dessiner_sol_milieu_niv1(-500.0f*9, -500.0f*8);
  //init_passage_droite(-500.0f*9, -500.0f*9 +250.0f);
  dessiner_sol_normal_niv1(-500.0f*10, -500.0f*9);
  dessiner_sol_gauche_niv1(-500.0f*11, -500.0f*10);
  dessiner_sol_milieu_niv1(-500.0f*12, -500.0f*11);
  dessiner_sol_gauche_niv1(-500.0f*13, -500.0f*12);
  dessiner_sol_normal_niv1(-500.0f*14, -500.0f*13);
  dessiner_sol_droite_niv1(-500.0f*15, -500.0f*14);
  dessiner_sol_milieu_niv1(-500.0f*16, -500.0f*15);
  dessiner_sol_gauche_niv1(-500.0f*17, -500.0f*16);
  dessiner_sol_normal_niv1(-500.0f*18, -500.0f*17);
  dessiner_sol_milieu_niv1(-500.0f*19, -500.0f*18);
  //init_passage_droite(-500.0f*19, -500.0f*19 +250.0f);
  dessiner_sol_normal_niv1(-500.0f*20, -500.0f*19);
  dessiner_sol_milieu_niv1(-500.0f*21, -500.0f*20);
  dessiner_sol_droite_niv1(-500.0f*22, -500.0f*21);
  dessiner_sol_droite_niv1(-500.0f*23, -500.0f*22);
  dessiner_sol_normal_niv1(-500.0f*24, -500.0f*23);
  dessiner_sol_milieu_niv1(-500.0f*25, -500.0f*24);
  //init_passage_gauche(-500.0f*25, -500.0f*25 +250.0f);
  dessiner_sol_normal_niv1(-500.0f*26, -500.0f*25);
  dessiner_sol_milieu_niv1(-500.0f*27, -500.0f*26);
  dessiner_sol_gauche_niv1(-500.0f*28, -500.0f*27);
  dessiner_sol_gauche_niv1(-500.0f*29, -500.0f*28);
  dessiner_sol_normal_niv1(-500.0f*30, -500.0f*29);
  dessiner_passages();


  
  dessiner_sol_normal_niv2(-500.0f, 30.0f);
  dessiner_sol_droite_niv2(-500.0f*2, -500.0f);
  dessiner_sol_milieu_niv2(-500.0f*3, -500.0f*2);
  dessiner_sol_gauche_niv2(-500.0f*4, -500.0f*3);
  dessiner_sol_normal_niv2(-500.0f*5, -500.0f*4);
  dessiner_sol_milieu_niv2(-500.0f*6, -500.0f*5);
  dessiner_sol_normal_niv2(-500.0f*7, -500.0f*6);
  dessiner_sol_gauche_niv2(-500.0f*8, -500.0f*7);
  dessiner_sol_milieu_niv2(-500.0f*9, -500.0f*8);
  dessiner_sol_normal_niv2(-500.0f*10, -500.0f*9);
  dessiner_sol_gauche_niv2(-500.0f*11, -500.0f*10);
  dessiner_sol_milieu_niv2(-500.0f*12, -500.0f*11);
  dessiner_sol_gauche_niv2(-500.0f*13, -500.0f*12);
  dessiner_sol_normal_niv2(-500.0f*14, -500.0f*13);
  dessiner_sol_droite_niv2(-500.0f*15, -500.0f*14);
  dessiner_sol_milieu_niv2(-500.0f*16, -500.0f*15);
  dessiner_sol_gauche_niv2(-500.0f*17, -500.0f*16);
  dessiner_sol_normal_niv2(-500.0f*18, -500.0f*17);
  dessiner_sol_milieu_niv2(-500.0f*19, -500.0f*18);
  dessiner_sol_normal_niv2(-500.0f*20, -500.0f*19);
  dessiner_sol_milieu_niv2(-500.0f*21, -500.0f*20);
  dessiner_sol_droite_niv2(-500.0f*22, -500.0f*21);
  dessiner_sol_droite_niv2(-500.0f*23, -500.0f*22);
  dessiner_sol_normal_niv2(-500.0f*24, -500.0f*23);
  dessiner_sol_milieu_niv2(-500.0f*25, -500.0f*24);
  dessiner_sol_normal_niv2(-500.0f*26, -500.0f*25);
  dessiner_sol_milieu_niv2(-500.0f*27, -500.0f*26);
  dessiner_sol_gauche_niv2(-500.0f*28, -500.0f*27);
  dessiner_sol_gauche_niv2(-500.0f*29, -500.0f*28);
  dessiner_sol_normal_niv2(-500.0f*30, -500.0f*29);
  
  for (int i = 0; i < nbr_obstacle; i+=2)
    animer_obstacle(i);  
  dessiner_obstacle(list_obstacle, 1);
  dessiner_obstacle(list_obstacle_niv_2, 2);
  dessiner_bonus(list_bonus, 1);
  dessiner_bonus(list_bonus_niv_2, 2);
  dessiner_bonus_vitesse();
  printf("pos Y = %f\n", dragon[0].centre_y);

  /* glColor3f(1.0f, 0.0f, 0.0f); */
  /* glutWireTorus(100.0f, 10.0f, 20, 20); */


  glutSwapBuffers();
  //glutPostRedisplay();
}



void gestionClavierSpec(int key, int x, int y){
  switch(key){
  case GLUT_KEY_LEFT:
    tourner_dragon_gauche();
    break;
  case GLUT_KEY_RIGHT:
    tourner_dragon_droite();
    break;
  case GLUT_KEY_UP:
    tourner_dragon_haut();
    break;
  case GLUT_KEY_DOWN:
    tourner_dragon_bas();
    break;
  }
}

void gestionClavier(unsigned char key, int x, int y){
  switch (key){
  case 27:
    exit(0);
    break;
  case ' ':
    if (bonus_vitesse_joueur > 0){
      dragon[0].centre_z -= 10.0f;
      bonus_vitesse_joueur--;
    }
    break;
  case 'd':
    avancer_dragon();
    break;
  case 'z':
    
    break;
  case 's':
    
    break;
  }    
}



int main(int argc, char **argv) {

  // initialiser rand
  srand(getpid());

  // init GLUT and create window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowPosition(250,250);
  glutInitWindowSize(700,500);
  glutCreateWindow("Dragon");

  // register callbacks
  glutDisplayFunc(renderScene);
  glutReshapeFunc(changeSize);
  glutIdleFunc(animer);
  glutKeyboardFunc(gestionClavier);
  glutSpecialFunc(gestionClavierSpec);

  // init
  glEnable(GL_DEPTH_TEST);
  init_dragon(dragon, taille_dragon);
  init_obstacle_alea(5);
  init_obstacle_niv_2_alea(4);
  /* afficher_obstacles(); */
  init_bonus_alea(8);
  init_bonus_niv_2_alea(8);
  //afficher_bonus();
  init_bonus_vitesse_alea(2);
  init_passage_gauche(-500.0f*6, -500.0f*6 +250.0f);
  init_passage_droite(-500.0f*9, -500.0f*9 +250.0f);
  init_passage_droite(-500.0f*19, -500.0f*19 +250.0f);
  init_passage_gauche(-500.0f*25, -500.0f*25 +250.0f);
	
  // enter GLUT event processing cycle
  glutMainLoop();

  return 1;
}
