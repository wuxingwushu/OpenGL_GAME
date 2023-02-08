#include "block.h"
#include "../Generate/PerlinNoise.h"//柏林噪声



PerlinNoise* Perlin = new PerlinNoise();

float Box[] =
{
    //X     Z      Y     //      向量
    //后面
    1.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    1.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    0.0f,  0.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    0.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    1.0f,  1.0f,  0.0f,  0.0f,  0.0f, -1.0f, 
    //正面
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    1.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
    //左面
    0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  0.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    0.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f,
    //右面
    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  1.0f,  1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
     //下面
    0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    1.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    1.0f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    0.0f,  0.0f,  1.0f,  0.0f, -1.0f,  0.0f,
    0.0f,  0.0f,  0.0f,  0.0f, -1.0f,  0.0f,
    //上面
    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
    1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f,
};


float Box1[] =
{
    //x,z,y
      0,0,0,
      1,0,0,
      1,1,0,
      0,1,0,

      0,0,1,
      1,0,1,
      1,1,1,
      0,1,1,
};

int index1[] =
{
    //前
    0,2,1,
    0,3,2,
    //后
    4,5,6,
    4,6,7,
    //下
    0,5,4,
    0,1,5,
    //上
    3,6,2,
    3,7,6,
    //右
    1,2,6,
    1,6,5,
    //左
    0,7,3,
    0,4,7,
};




block::block()
{
}

block::~block()
{

}



void block::blockinit(int x, int y, int z)
{
    /*
    int shuliang = 258;
    vector<vector<vector<int> > > data(shuliang);//创建2个vector<vector<int> >类型的数组

    for (int i = 0; i < shuliang; i++)
    {
        data[i].resize(shuliang);
        for (int j = 0; j < shuliang; j++)
        {
            data[i][j].resize(shuliang);
        }//之后才能对三维数组设定大小，否则内存出错
    }
    */
    /**/
    for (int k = 0; k < shuliang + 1; k++)
    {
        for (int w = 0; w < shuliang + 1; w++)
        { 
            int gd = Perlin->noise((x + k) / float(3500), (y + w) / float(3500), 0.6) * 1024;
            for (int g = 0; g < shuliang + 1; g++)
            {
                int she = 20 + rand() % 30;
                if ((g + z) < gd && (g + z) > gd - 5)DATAA[k][w][g] = 1 + (10 + (rand()%50)) * 65536 + 100 * 256 + (10 + (rand()%50));
                else if ((g + z) < gd - 4)DATAA[k][w][g] = 0 + she * 65536 + she * 256 + she;
                else DATAA[k][w][g] = 0;
                if(DATAA[k][w][g] > 0)if (Perlin->noise((x + k) / float(64), (y + w) / float(64), (z + g) / float(64)) < 0.3)DATAA[k][w][g] = 0;
            }
        }
    }
    
    //float* output = new float[100]();
    //ofstream fout("test.bin", ios::binary);
    //fout.write((char*)(&output[0]), sizeof(float));
    //fout.close();

    shengchengblock();
}

void block::shengchengblock()
{
    //short int
    vector<float> shu_vertex;
    shu_vertex.clear();
    for (int x = 1; x < shuliang; x++)
    {
        for (int y = 1; y < shuliang; y++)
        {
            for (int z = 1; z < shuliang; z++)
            {
                //printf("%dx,%dy,%dz", x, y, z);
                if(DATAA[x][y][z] != 0)
                {
                    float R = (DATAA[x][y][z] % 256) / 256.0f;
                    float G = ((DATAA[x][y][z] % 65536) / 256) / 256.0f;
                    float B = ((DATAA[x][y][z] % 16777216) / 65536) / 256.0f;
                    for (int face = 0; face < 6; face++)
                    {
                        bool kai = true;
                        switch (face)
                        {
                        case 0:if (DATAA[x][y - 1][z] != 0)kai = false; break;
                        case 1:if (DATAA[x][y + 1][z] != 0)kai = false; break;
                        case 2:if (DATAA[x - 1][y][z] != 0)kai = false; break;
                        case 3:if (DATAA[x + 1][y][z] != 0)kai = false; break;
                        case 4:if (DATAA[x][y][z - 1] != 0)kai = false; break;
                        case 5:if (DATAA[x][y][z + 1] != 0)kai = false; break;
                        default:break;
                        }
                        if (kai)
                        {
                            for (int dian = 0; dian < 6; dian++)
                            {
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 0] + x - 1);            //位置
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 1] + z - 1);
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 2] + y - 1);
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 3]);                //法向量
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 4]);
                                shu_vertex.push_back(Box[face * 36 + dian * 6 + 5]);
                                shu_vertex.push_back(R);                                            //颜色
                                shu_vertex.push_back(G);
                                shu_vertex.push_back(B);
                            }
                        }
                    }
                }
            }
        }
    }
    float* vertex = new float[shu_vertex.size()];
    copy(shu_vertex.begin(), shu_vertex.end(), vertex);

    DATA.vertexsizeof = shu_vertex.size() * sizeof(float);
    DATA.vertex = vertex;
    DATA.shu = (shu_vertex.size() / 3);
    //printf("%d - %f", DATA.vertexsizeof, DATA.vertex);
    shu_vertex.clear();
}

void block::pohuai(int x, int y, int z, int px, int py, int pz)
{
    if (px < 0)x = jiaozhen(x);
    if (py < 0)y = jiaozhen(y);
    if (pz < 0)z = jiaozhen(z);
    if (x >= 0 && x <= shuliang && y >= 0 && y <= shuliang && z >= 0 && z <= shuliang)
    {
        if (DATAA[x + 1][z + 1][y + 1] != 0)
        {
            DATAA[x + 1][z + 1][y + 1] = 0;
        }
        shengchengblock();
    }
}

void block::fangzhi(int x, int y, int z, int px, int py, int pz)
{
    if (px < 0)x = jiaozhen(x);
    if (py < 0)y = jiaozhen(y);
    if (pz < 0)z = jiaozhen(z);
    if (x >= 0 && x <= shuliang && y >= 0 && y <= shuliang && z >= 0 && z <= shuliang)
    {
        if (DATAA[x + 1][z + 1][y + 1] == 0)
        {
            DATAA[x + 1][z + 1][y + 1] = 1 + 200 * 65536 + 20 * 256 + 20;
        }
        shengchengblock();
    }
}

int block::jiaozhen(int j)
{
    if (j <= 0)j += shuliang-2;
    return j;
}

void block::blockgengxin(int x, int y, int z)
{
    DATAA[x + 1][z + 1][y + 1] = 0;
    shengchengblock();
}

void block::blockgengxiny(int x, int y, int z, int lei)
{
    DATAA[x + 1][z + 1][y + 1] = lei;
    shengchengblock();
}
