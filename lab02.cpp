#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <GL/glut.h>
using namespace std;

int v_num = 0; //��¼�������
int f_num = 0; //��¼�������
int vn_num = 0;

GLfloat **vArr; //��ŵ�Ķ�ά����
int **fvArr; //����涥��Ķ�ά����
int **fnArr;
GLfloat **vnArr;
string s1, s2, s3, s4;
int i1;
GLfloat f2, f3, f4;

float normal[426][3] = { 0 };      //���㷨������Ϣ��Ϊ��λ������
float angle[426] = { 0 };        //�������㷨�����ĽǶ���Ϣ�ĸ�������

float viewpoint_x = 0;         //��¼�ӵ�λ��
float viewpoint_y = 0;
float viewpoint_z = 2;
float light_x = 0;             //��¼��Դλ��
float light_y = 0;
float light_z = 0;
float rotate_x = 0;            //��¼��ת�Ƕ�
float rotate_y = 0;
float rotate_z = 0;
bool l = false;                //��ǰģʽΪ��Դ�ƶ�
bool v = false;                //��ǰģʽΪ�ӵ��ƶ�
bool r = false;                //��ǰģʽΪ��ת  

// ���ڼ��㶥�㷨�����ĺ���
void calc_n_vector()
{
	float n1[3], n2[3];        //��ʱ����
	float n[3];                //������
	float a;                   //����Ƕ�
	float l1, l2;              //��ʱ��������
	int i, j;
	float x, y, z, r;             //���㷨�������꼰����
								  //���������㹫ʽΪn=(n1*a1+n2*a2+...)/(a1+a2+...)
								  //aiΪ�������������εĶ���Ƕȣ����ȣ���niΪ�������������εķ�����
	for (i = 0; i < 604; i++)
	{
		n1[0] = vArr[fvArr[i][1] - 1][0] - vArr[fvArr[i][0] - 1][0];
		n1[1] = vArr[fvArr[i][1] - 1][1] - vArr[fvArr[i][0] - 1][1];
		n1[2] = vArr[fvArr[i][1] - 1][2] - vArr[fvArr[i][0] - 1][2];
		n2[0] = vArr[fvArr[i][2] - 1][0] - vArr[fvArr[i][1] - 1][0];
		n2[1] = vArr[fvArr[i][2] - 1][1] - vArr[fvArr[i][1] - 1][1];
		n2[2] = vArr[fvArr[i][2] - 1][2] - vArr[fvArr[i][1] - 1][2];
		//���������С�����·�������С��Ϊ��ֹ������㾫�Ȳ�������ͳһ����
		n[0] = 100 * (n1[1] * n2[2] - n2[1] * n1[2]);
		n[1] = 100 * (n1[2] * n2[0] - n2[2] * n1[0]);
		n[2] = 100 * (n1[0] * n2[1] - n2[0] * n1[1]);
		//ai�ļ��㹫ʽΪa=arccos((pa*pb)/|pa||pb|)���˴�*Ϊ���
		for (j = 0; j < 3; j++)
		{
			n1[0] = vArr[fvArr[i][(j + 2) % 3] - 1][0] - vArr[fvArr[i][j] - 1][0];
			n1[1] = vArr[fvArr[i][(j + 2) % 3] - 1][1] - vArr[fvArr[i][j] - 1][1];
			n1[2] = vArr[fvArr[i][(j + 2) % 3] - 1][2] - vArr[fvArr[i][j] - 1][2];
			l1 = sqrtf(n1[0] * n1[0] + n1[1] * n1[1] + n1[2] * n1[2]);
			n2[0] = vArr[fvArr[i][(j + 1) % 3] - 1][0] - vArr[fvArr[i][j] - 1][0];
			n2[1] = vArr[fvArr[i][(j + 1) % 3] - 1][1] - vArr[fvArr[i][j] - 1][1];
			n2[2] = vArr[fvArr[i][(j + 1) % 3] - 1][2] - vArr[fvArr[i][j] - 1][2];
			l2 = sqrtf(n2[0] * n2[0] + n2[1] * n2[1] + n2[2] * n2[2]);
			a = acosf((n1[0] * n2[0] + n1[1] * n2[1] + n1[2] * n2[2]) / (l1*l2));
			normal[fvArr[i][j] - 1][0] += n[0] * a;
			normal[fvArr[i][j] - 1][1] += n[1] * a;
			normal[fvArr[i][j] - 1][2] += n[2] * a;
			angle[fvArr[i][j] - 1] += a;
		}
	}
	//���㶥�㷨�������ս��������Ϊ��λ����
	for (i = 0; i < 426; i++)
	{
		x = normal[i][0] / angle[i];
		y = normal[i][1] / angle[i];
		z = normal[i][2] / angle[i];
		r = sqrtf(x*x + y*y + z*z);
		normal[i][0] = x / r;
		normal[i][1] = y / r;
		normal[i][2] = z / r;
	}
}


// �����Ĺ�Դ���ú������趨��Դ��λ�ÿ�ͨ�����̸ı�
void setlight()
{
	float light_position[4] = { light_x,light_y,light_z,1.0 };     //��Դλ��
	float light_ambient[4] = { 0,0,0,1.0 };                       //��Դ�����⣬��ɫ������
	float light_diffuse[4] = { 1.0,1.0,1.0,1.0 };                 //��Դ������⣬��ɫ
	float light_specular[4] = { 1.0,1.0,1.0,1.0 };                //��Դ����⣬��ɫ
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glEnable(GL_LIGHT0);                                          //������Դ
	glEnable(GL_LIGHTING);
}



void getLineNum(string addrstr) //��ȡ����������
{
	ifstream infile(addrstr.c_str()); //��ָ���ļ�
	string sline;//ÿһ��
	int i = 0, j = 0;

	while (getline(infile, sline)) //��ָ���ļ����ж�ȡ
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')
				vn_num++;
			else
				v_num++;
		}
		if (sline[0] == 'f')
		{
			f_num++;
		}
	}
}
int readfile(string addrstr) //���ļ����ݶ���������ȥ
{
	vArr = new GLfloat*[v_num];
	for (int i = 0; i<v_num; i++)
	{
		vArr[i] = new GLfloat[3];
	}
	vnArr = new GLfloat*[vn_num];
	for (int i = 0; i<vn_num; i++)
	{
		vnArr[i] = new GLfloat[3];
	}
	fvArr = new int*[f_num];
	fnArr = new int*[f_num];
	for (int i = 0; i<f_num; i++)
	{
		fvArr[i] = new int[3];
		fnArr[i] = new int[3];
	}
	ifstream infile(addrstr.c_str());
	string sline;//ÿһ��
	int ii = 0, jj = 0, kk = 0;

	// ���ж�ȡ�ļ���Ϣ
	while (getline(infile, sline))
	{
		if (sline[0] == 'v')
		{
			if (sline[1] == 'n')//vn
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vnArr[ii][0] = f2;
				vnArr[ii][1] = f3;
				vnArr[ii][2] = f4;
				ii++;
			}
			else//v
			{
				istringstream sin(sline);
				sin >> s1 >> f2 >> f3 >> f4;
				vArr[jj][0] = f2;
				vArr[jj][1] = f3;
				vArr[jj][2] = f4;
				jj++;
			}
		}
		if (sline[0] == 'f') //�洢��
		{
			istringstream in(sline);
			// GLfloat a;
			in >> s1;
			// int i, k;
			for (int i = 0; i<3; i++)
			{
				in >> i1;
				cout << i1 << endl;
				//ȡ����һ������
				fvArr[kk][i] = i1;
			}
			kk++;
		}
	}
	return 0;
}

void init(void)
{
	getLineNum("D:\\tshirtw.obj");
	readfile("D:\\tshirtw.obj");

	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 50.0 };
	GLfloat light_position[] = { 1.0, 1.0f, 1.0, 0.0 };
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_SMOOTH);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	glShadeModel(GL_SMOOTH);
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);				// Black Background
}

// ģ�ͻ��ƺ���
void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);  //������ȼ�⣬�����ڵ�Ч��
	glDepthFunc(GL_LEQUAL);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	//�����ӵ㣬�ӵ�λ�ÿ�ͨ�����̸ı�
	glLoadIdentity();
	gluPerspective(60.0f, 1.0f, 1.0f, 10.0f);
	gluLookAt(viewpoint_x, viewpoint_y, viewpoint_z, 0, 0, 0, 0, 1, 0);

	setlight();

	glRotatef(rotate_x, 1, 0, 0);
	glRotatef(rotate_y, 0, 1, 0);
	glRotatef(rotate_z, 0, 0, 1);

	float ts_ambient[4] = { 1,1,0,1.0 };       //���廷���⣬��ɫ
	float ts_diffuse[4] = { 1,1,1,1.0 };       //����������⣬��ɫ
	float ts_specular[4] = { 1,1,1,1.0 };      //���徵�淴��⣬��ɫ
	float ts_emission[4] = { 0,0,0,1.0 };      //�������⣬��ɫ����������
	float ts_shininess = 30.0;                 //�������徵������

	//���������������
	glMaterialfv(GL_FRONT, GL_AMBIENT, ts_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ts_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ts_specular);
	glMaterialfv(GL_FRONT, GL_EMISSION, ts_emission);
	glMaterialf(GL_FRONT, GL_SHININESS, ts_shininess);

	for (int i = 0; i < f_num; i++)
	{
		// glBegin(GL_TRIANGLES);
		glBegin(GL_TRIANGLE_STRIP);
		// glBegin(GL_LINE_LOOP);

		//glNormal3f(vnArr[fnArr[i][0]-1][0], vnArr[fnArr[i][0]-1][1], vnArr[fnArr[i][0]-1][2]);
		glVertex3f(vArr[fvArr[i][0] - 1][0], vArr[fvArr[i][0] - 1][1], vArr[fvArr[i][0] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][1]-1][0], vnArr[fnArr[i][1]-1][1], vnArr[fnArr[i][1]-1][2]);
		glVertex3f(vArr[fvArr[i][1] - 1][0], vArr[fvArr[i][1] - 1][1], vArr[fvArr[i][1] - 1][2]);

		//glNormal3f(vnArr[fnArr[i][2]-1][0], vnArr[fnArr[i][2]-1][1], vnArr[fnArr[i][2]-1][2]);
		glVertex3f(vArr[fvArr[i][2] - 1][0], vArr[fvArr[i][2] - 1][1], vArr[fvArr[i][2] - 1][2]);

		for (int j = 0; j < 3; j++)
		{
			glNormal3fv(normal[fvArr[i][j] - 1]);
			glVertex3fv(vArr[fvArr[i][j] - 1]);
		}
		glEnd();

	}
	glFlush();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);	
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w <= h)
		glOrtho(-1.5, 1.5, -1.5 * (GLfloat)h / (GLfloat)w, 1.5 * (GLfloat)h / (GLfloat)w, -10.0, 10.0);
	else
		glOrtho(-1.5*(GLfloat)w / (GLfloat)h, 1.5*(GLfloat)w / (GLfloat)h, -1.5, 1.5, -10.0, 10.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

/* ������Ӧ���� */
void mykeyboard(unsigned char key, int x, int y)        
{
	/* ��Ӧ��������
	 *
	 * L(light)����Դ�ƶ�ģʽ
	 * V(view)���ӵ��ƶ�ģʽ
	 * R(rotate)����תģ��
	 *
	 * Q����x���������ƶ�
	 * A����x�Ḻ�����ƶ�
	 * W����y���������ƶ�
	 * S����y�Ḻ�����ƶ�
	 * E����z���������ƶ�
	 * D����z�Ḻ�����ƶ�
	 */
	switch (key)
	{
	case 'l':l = true; v = false; r = false; break;
	case 'v':l = false; v = true; r = false; break;
	case 'r':l = false; v = false; r = true; break;
	case 'e':if (l)light_z += 0.2; 
		if (v)viewpoint_z += 0.2;
		if (r)rotate_z += 1;
		break;
	case 'd':if (l)light_z -= 0.2;
		if (v)viewpoint_z -= 0.2;
		if (r)rotate_z -= 1;
		break;
	case 'w':if (l)light_y += 0.2;
		if (v)viewpoint_y += 0.2;
		if (r)rotate_y += 1;
		break;
	case 's':if (l)light_y -= 0.2;
		if (v)viewpoint_y -= 0.2;
		if (r)rotate_y -= 1;
		break;
	case 'q':if (l)light_x += 0.2;
		if (v)viewpoint_x += 0.2;
		if (r)rotate_x += 1;
		break;
	case 'a':if (l)light_x -= 0.2;
		if (v)viewpoint_x -= 0.2;
		if (r)rotate_x -= 1;
		break;
	default:break;
	}
	glutPostRedisplay();//���»���
}



int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(600, 800);
	glutInitWindowPosition(100, 100);
	glutCreateWindow(argv[0]);
	init();
	calc_n_vector();
	glutKeyboardFunc(mykeyboard);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMainLoop();
	return 0;
}
