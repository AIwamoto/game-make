#include <GL/glut.h>
#include <GL/glpng.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define ON 1
#define OFF 0


void Display(void);
void Reshape(int, int);
void Timer(int);
void PutSprite(int, int, int, pngInfo *);
void PutSprite_reset(int, int, int, pngInfo *);
void PutSprite_clear(int, int, int, pngInfo *);
void Mouse(int, int, int, int);
void PassiveMotion(int, int);
void Motion(int, int);
void Entry(int);
void make_board(void);
void PanelSwitch(int *);

GLuint img[4];
pngInfo info[4];
int board[5][5];
int xsign[5], ysign[5];

int main(int argc, char **argv){
  int i;
  char fname[20];

  srand(time(NULL));
  make_board();

  glutInit(&argc, argv);
  glutInitWindowSize(320, 384);
  glutCreateWindow("lightsout");
  glutInitDisplayMode(GLUT_RGBA);
  glClearColor(0.0, 0.0, 1.0, 1.0);

  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

  for(i = 0; i<4; i++){
    sprintf(fname, "lightsout_%02d.png", i);

    img[i] = pngBind(fname, PNG_NOMIPMAP, PNG_ALPHA, &info[i], GL_CLAMP, GL_NEAREST, GL_NEAREST);
  }

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutTimerFunc(1000, Timer, 0);
  glutMouseFunc(Mouse);
  glutPassiveMotionFunc(PassiveMotion);
  glutMotionFunc(Motion);
  glutEntryFunc(Entry);

  glutMainLoop();


  return 0;
}


void Display(void){
  int i, j, counter = 0;
  int x, y, w, h;

  w = glutGet(GLUT_WINDOW_WIDTH)/5;
  h = glutGet(GLUT_WINDOW_HEIGHT)/6;


  glClear(GL_COLOR_BUFFER_BIT);

  for(i = 0; i<5; i++){
    for(j = 0; j<5; j++){
      if(board[i][j] == ON){
        x = w*j;
        y = h*i;
        PutSprite(img[1], x, y, &info[1]);
      }
      else if(board[i][j] == OFF){
        x = w*j;
        y = h*i;
        PutSprite(img[0], x, y, &info[0]);
      }
    }
  }
  PutSprite_reset(img[2], 0, h*5, &info[2]);

  for(i = 0; i<5; i++){
    for(j = 0; j<5; j++){
      counter += board[i][j];
    }
  }
  if(counter == 25 || counter == 0){
    PutSprite_clear(img[3], 0, 0, &info[3]);
  }

  glFlush();
}


void Reshape(int w, int h){
  glViewport(0, 0, w, h);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glScaled(1, -1, 1);
  glTranslated(0, -h, 0);
}

void Timer(int t){
  glutPostRedisplay();
  glutTimerFunc(500, Timer, 0);
}

void PutSprite(int num, int x, int y, pngInfo *info){
  int w, h;

  w = glutGet(GLUT_WINDOW_WIDTH)/5;
  h = glutGet(GLUT_WINDOW_HEIGHT)/6;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(255, 255, 255, 255);

  glBegin(GL_QUADS);

  glTexCoord2i(0, 0);
  glVertex2i(x, y);

  glTexCoord2i(0, 1);
  glVertex2i(x, y + h);

  glTexCoord2i(1, 1);
  glVertex2i(x + w, y + h);

  glTexCoord2i(1, 0);
  glVertex2i(x + w, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void PutSprite_reset(int num, int x, int y, pngInfo *info){
  int w, h;

  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT)/6;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(255, 255, 255, 255);

  glBegin(GL_QUADS);

  glTexCoord2i(0, 0);
  glVertex2i(x, y);

  glTexCoord2i(0, 1);
  glVertex2i(x, y + h);

  glTexCoord2i(1, 1);
  glVertex2i(x + w, y + h);

  glTexCoord2i(1, 0);
  glVertex2i(x + w, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void PutSprite_clear(int num, int x, int y, pngInfo *info){
  int w, h;

  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT)/6*5;

  glPushMatrix();
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, num);
  glColor4ub(255, 255, 255, 255);

  glBegin(GL_QUADS);

  glTexCoord2i(0, 0);
  glVertex2i(x, y);

  glTexCoord2i(0, 1);
  glVertex2i(x, y + h);

  glTexCoord2i(1, 1);
  glVertex2i(x + w, y + h);

  glTexCoord2i(1, 0);
  glVertex2i(x + w, y);

  glEnd();

  glDisable(GL_TEXTURE_2D);
  glPopMatrix();
}

void Mouse(int b, int s, int x, int y){
  int w, h, xx, yy;

  w = glutGet(GLUT_WINDOW_WIDTH);
  h = glutGet(GLUT_WINDOW_HEIGHT);
  xx = (int)((double)x/w*5);
  yy = (int)((double)y/h*6);

  if(b == GLUT_LEFT_BUTTON){
    if(s == GLUT_DOWN){
      if(yy < 5){
        PanelSwitch(&board[yy][xx]);
        if(yy > 0){
          PanelSwitch(&board[yy-1][xx]);
        }
        if(yy < 4){
          PanelSwitch(&board[yy+1][xx]);
        }
        if(xx > 0){
          PanelSwitch(&board[yy][xx-1]);
        }
        if(xx < 4){
          PanelSwitch(&board[yy][xx+1]);
        }
      }
      if(yy >= 5){
        make_board();
      }
    }
  }
}


void PassiveMotion(int x, int y){
}


void Motion(int x, int y){
}


void Entry(int s){
}

void make_board(void){
  int i, j, k;

  for(i = 0; i<5; i++){
    for(j = 0; j<5; j++){
      board[i][j] = 0;
    }
  }

  for(i = 0; i<5; i++){
    xsign[i] = (int)(((float)rand()/RAND_MAX*5) - 0.5);
    if(xsign[i] < 0) xsign[i] = 0;
    ysign[i] = (int)(((float)rand()/RAND_MAX*5) - 0.5);
    if(ysign[i] < 0) ysign[i] = 0;
  }

  for(i = 0; i<5; i++){ // y座標
    for(j = 0; j<5; j++){ // x座標
      for(k = 0; k<5; k++){
        if(j == xsign[k] && i == ysign[k]){
          if(board[i][j] == ON){
            xsign[k] = (int)(((float)rand()/RAND_MAX*5) - 0.5);
            if(xsign[k] < 0) xsign[k] = 0;
            ysign[k] = (int)(((float)rand()/RAND_MAX*5) - 0.5);
            if(ysign[k] < 0) ysign[k] = 0;
          }
          board[ysign[k]][xsign[k]] = ON;
        }
      }
    }
  }
}
void PanelSwitch(int *s){
  if(*s == ON){
    *s = OFF;
  }
  else if(*s == OFF){
    *s = ON;
  }
}
