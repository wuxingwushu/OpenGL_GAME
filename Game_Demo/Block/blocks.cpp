#include "blocks.h"
#include <iostream>
blocks::blocks()
{
}
void blocks::Threadblockinit(int a, int b)
{
    for (int ix = a; ix < b; ix++)
    {
        blockss[ix]->blockinit((ix % bianshu + Blockposx) * blockboxshu, ((ix / bianshu) % bianshu + Blockposy) * blockboxshu, ((ix / bianshu_2) - pos_pianyi + Blockposz) * blockboxshu);
    }
}
//初始话
void blocks::init(double camerax, double cameray, double cameraz)
{
    //auto lambAdd = [](block* block, int a, int bianshu, int bianshu_2, int pos_pianyi)
    //{ block->blockinit((a % bianshu) * 64, ((a / bianshu) % bianshu) * 64, ((a / bianshu_2) - pos_pianyi) * 64); };
    for (int ix = 0; ix < blockshuliang; ix++)
    {
        blockss[ix] = (block*)operator new(sizeof(block));
        new(blockss[ix]) block();
        //blockss[ix]->blockinit((ix % bianshu) * 64, ((ix / bianshu) % bianshu) * 64, ((ix / bianshu_2) - pos_pianyi) * 64);
    }
    blockboxshu = blockss[0]->shuliang - 1;
    shiye_pianyi = (pos_pianyi - 1) * blockboxshu - 1;
    shiye_pianyi_ban = shiye_pianyi;

    Blockposx = int(camerax / blockboxshu);
    Blockposy = int(cameraz / blockboxshu);
    Blockposz = int(cameray / blockboxshu);

    std::vector<std::thread> Threads;

    int shuliangbox = blockshuliang / threadshu;
    for (int ix = 0; ix < threadshu -1; ix++)
    {
        Threads.push_back(std::thread(&blocks::Threadblockinit,this, ix* shuliangbox,(ix+1)* shuliangbox));
    }
    Threads.push_back(std::thread(&blocks::Threadblockinit, this, (threadshu-1) * shuliangbox, blockshuliang));
    for (int ix = 0; ix < threadshu; ix++)
    {
        Threads[ix].join();
    }
    /*
    std::vector<std::thread> Threads;
    bool Threads_kai = true;
    int blockss_i = 0;
    int xxxcc;
    double time_d;
    int xiancshu = 10;
    while (Threads_kai) {
        time_d = glfwGetTime();
        for (int ix = 0; ix < xiancshu; ix++)
        {
            Threads.push_back(std::thread(&block::blockinit, blockss[blockss_i + ix], ((blockss_i + ix) % bianshu) * 64, (((blockss_i + ix) / bianshu) % bianshu) * 64, (((blockss_i + ix) / bianshu_2) - pos_pianyi) * 64));
            xxxcc = ix + 1;
            if ((blockss_i + xxxcc) >= blockshuliang)break;
        }
        double time_dx = (glfwGetTime() - time_d)/ xiancshu;
        std::cout << "haofei  :   " << time_dx << std::endl;
        for (int ix = 0; ix < xxxcc; ix++)
        {
            Threads[blockss_i + ix].join();
        }
        blockss_i = blockss_i + xxxcc;
        //std::cout << "xxxxxx:   " << blockss_i << std::endl;
        if ((blockss_i) >= blockshuliang) { Threads_kai = false; break; }
    }*/


    for (int i = 0; i < blockshuliang; i++)
    {
        //std::cout << i << blockss[i]->DATA.vertexsizeof << std::endl;
        glGenVertexArrays(1, &VAOBOX[i]);
        glGenBuffers(1, &VBOBOX[i]);
        // 1. 绑定顶点数组对象
        glBindVertexArray(VAOBOX[i]);
        // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
        glBindBuffer(GL_ARRAY_BUFFER, VBOBOX[i]); 
        //printf("%d - %f", blockss[i]->DATA.vertexsizeof, blockss[i]->DATA.vertex);
        glBufferData(GL_ARRAY_BUFFER, blockss[i]->DATA.vertexsizeof, blockss[i]->DATA.vertex, GL_STATIC_DRAW);
        delete[]blockss[i]->DATA.vertex;
        // 4. 设定顶点属性指针
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2); 
    }
    /*
    Blockposx = camerapos.x / blockboxshu;
    Blockposy = camerapos.y / blockboxshu;
    Blockposz = camerapos.z / blockboxshu;
    */
    //定义三维动态数组，并赋予初始值tuen;
    Blockkong = new bool** [bianshu];
    for (size_t y = 0; y < bianshu; y++)
    {
        Blockkong[y] = new bool* [bianshu];
        for (size_t x = 0; x < bianshu; x++)
        {
            Blockkong[y][x] = new bool[bianshu];
            for (size_t z = 0; z < bianshu; z++)
            {
                Blockkong[y][x][z] = 1;
            }
        }
    }
    shnegchen_Blockkong = new bool** [bianshu];
    for (size_t y = 0; y < bianshu; y++)
    {
        shnegchen_Blockkong[y] = new bool* [bianshu];
        for (size_t x = 0; x < bianshu; x++)
        {
            shnegchen_Blockkong[y][x] = new bool[bianshu];
            for (size_t z = 0; z < bianshu; z++)
            {
                shnegchen_Blockkong[y][x][z] = 0;
            }
        }
    }
}

//确认位置z
void blocks::POSjiaozhun(float x, float y, float z, int blockshui)
{
    int qx, qy, qz;
    if (int(x + 0.04f) != int(x - 0.04f))
    {
        if (cameraForward.x > 0)
        {
            qx = int(x + 0.04f);
        }
        else
        {
            qx = int(x - 0.04f);
        }
        blockss[blockshui]->pohuai(qx % blockboxshu, (int)y % blockboxshu, (int)z % blockboxshu, int(x), int(y), int(z));
        gengxinblock(qx % blockboxshu, (int)y % blockboxshu, (int)z % blockboxshu, blockshui, int(x), int(y), int(z));
    }
    else if (int(y + 0.04f) != int(y - 0.04f))
    {
        if (cameraForward.y > 0)
        {
            qy = int(y + 0.04f);
        }
        else
        {
            qy = int(y - 0.04f);
        }
        blockss[blockshui]->pohuai((int)x % blockboxshu, qy % blockboxshu, (int)z % blockboxshu, int(x), int(y), int(z));
        gengxinblock((int)x % blockboxshu, qy % blockboxshu, (int)z % blockboxshu, blockshui, int(x), int(y), int(z));
    }
    else
    {
        if (cameraForward.z > 0)
        {
            qz = int(z + 0.04f);
        }
        else
        {
            qz = int(z - 0.04f);
        }
        blockss[blockshui]->pohuai((int)x % blockboxshu, (int)y % blockboxshu, qz % blockboxshu, int(x), int(y), int(z));
        gengxinblock((int)x % blockboxshu, (int)y % blockboxshu, qz % blockboxshu, blockshui, int(x), int(y), int(z));
    }
}

//确认位置y
void blocks::POSjiaozhuny(float x, float y, float z, int blockshui)
{
    int qx, qy, qz;
    if (int(x + 0.04f) != int(x - 0.04f))
    {
        if (cameraForward.x > 0)
        {
            qx = int(x - 0.04f);
        }
        else
        {
            qx = int(x + 0.04f);
        }
        blockss[blockshui]->fangzhi(qx % blockboxshu, (int)y % blockboxshu, (int)z % blockboxshu, int(x), int(y), int(z));
        gengxinblocky(qx % blockboxshu, (int)y % blockboxshu, (int)z % blockboxshu, blockshui, int(x), int(y), int(z));
    }
    else if (int(y + 0.04f) != int(y - 0.04f))
    {
        if (cameraForward.y > 0)
        {
            qy = int(y - 0.04f);
        }
        else
        {
            qy = int(y + 0.04f);
        }
        blockss[blockshui]->fangzhi((int)x % blockboxshu, qy % blockboxshu, (int)z % blockboxshu, int(x), int(y), int(z));
        gengxinblocky((int)x % blockboxshu, qy % blockboxshu, (int)z % blockboxshu, blockshui, int(x), int(y), int(z));
    }
    else
    {
        if (cameraForward.z > 0)
        {
            qz = int(z - 0.04f);
        }
        else
        {
            qz = int(z + 0.04f);
        }
        blockss[blockshui]->fangzhi((int)x % blockboxshu, (int)y % blockboxshu, qz % blockboxshu, int(x), int(y), int(z));
        gengxinblocky((int)x % blockboxshu, (int)y % blockboxshu, qz % blockboxshu, blockshui, int(x), int(y), int(z));
    }
}

//区块移动
void blocks::blockPOS()
{
    double sjian = glfwGetTime();
    
    if (shengchen_kai) {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t x = 0; x < bianshu; x++)
            {
                for (size_t z = 0; z < bianshu; z++)
                {
                    if (shnegchen_Blockkong[x][y][z] == 1)
                    {
                        //blocks[x + y * bianshu + z * bianshu_2] = (block*)operator new(sizeof(block));
                        //new(blocks[x + y * bianshu + z * bianshu_2]) block();

                        //blockss[x + y * bianshu + z * bianshu_2]->blockinit((x + Blockposx) * 64, (y + Blockposy) * 64, (z + Blockposz - pos_pianyi) * 64);

                        //glGenVertexArrays(1, &VAOBOX[x + y * bianshu + z * bianshu_2]);
                        //glGenBuffers(1, &VBOBOX[x + y * bianshu + z * bianshu_2]);
                        // 1. 绑定顶点数组对象
                        glBindVertexArray(VAOBOX[x + y * bianshu + z * bianshu_2]);
                        // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
                        glBindBuffer(GL_ARRAY_BUFFER, VBOBOX[x + y * bianshu + z * bianshu_2]);
                        glBufferData(GL_ARRAY_BUFFER, blockss[x + y * bianshu + z * bianshu_2]->DATA.vertexsizeof, blockss[x + y * bianshu + z * bianshu_2]->DATA.vertex, GL_STATIC_DRAW);
                        delete[]blockss[x + y * bianshu + z * bianshu_2]->DATA.vertex;
                        // 4. 设定顶点属性指针
                        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
                        glEnableVertexAttribArray(0);
                        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
                        glEnableVertexAttribArray(1);
                        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
                        glEnableVertexAttribArray(2);

                        Blockkong[x][y][z] = 1;
                        shnegchen_Blockkong[x][y][z] = 0;
                    }
                }
            }
        }
        shengchen_kai = false;
    }
    //std::cout << "生成耗时  :   " << glfwGetTime() - sjian << std::endl;
}

void blocks::blockPOSshengchen() {
    if (int(camerapos.x / blockboxshu) - Blockposx == -1)
    {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                blockss[blockshuliang] = blockss[bianshu - 1 + y * bianshu + z * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[bianshu - 1 + y * bianshu + z * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[bianshu - 1 + y * bianshu + z * bianshu_2];
                //blocks[bianshu - 1 + y * bianshu + z * bianshu_2]->~block();
                //delete blocks[bianshu - 1 + y * bianshu + z * bianshu_2];
                for (size_t x = bianshu - 1; x > 0; x--)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x - 1 + y * bianshu + z * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x - 1 + y * bianshu + z * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x - 1 + y * bianshu + z * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x - 1][y][z];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x - 1][y][z];
                }
                blockss[0 + y * bianshu + z * bianshu_2] = blockss[blockshuliang];
                VAOBOX[0 + y * bianshu + z * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[0 + y * bianshu + z * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[0][y][z] = 0;
                shnegchen_Blockkong[0][y][z] = 0;
            }
        }
    }
    else if (int(camerapos.x / blockboxshu) - Blockposx == 1)
    {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                blockss[blockshuliang] = blockss[0 + y * bianshu + z * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[0 + y * bianshu + z * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[0 + y * bianshu + z * bianshu_2];
                //delete blocks[0 + y * bianshu + z * bianshu_2];
                for (size_t x = 0; x < bianshu - 1; x++)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x + 1 + y * bianshu + z * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x + 1 + y * bianshu + z * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x + 1 + y * bianshu + z * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x + 1][y][z];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x + 1][y][z];
                }
                blockss[bianshu - 1 + y * bianshu + z * bianshu_2] = blockss[blockshuliang];
                VAOBOX[bianshu - 1 + y * bianshu + z * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[bianshu - 1 + y * bianshu + z * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[bianshu - 1][y][z] = 0;
                shnegchen_Blockkong[bianshu - 1][y][z] = 0;
            }
        }
    }


    if (int(camerapos.z / blockboxshu) - Blockposy == -1)
    {
        for (size_t x = 0; x < bianshu; x++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                blockss[blockshuliang] = blockss[x + (bianshu - 1) * bianshu + z * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[x + (bianshu - 1) * bianshu + z * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[x + (bianshu - 1) * bianshu + z * bianshu_2];
                //delete blocks[x + (bianshu - 1) * bianshu + z * bianshu_2];
                for (size_t y = bianshu - 1; y > 0; y--)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x + (y - 1) * bianshu + z * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x + (y - 1) * bianshu + z * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x + (y - 1) * bianshu + z * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x][y - 1][z];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x][y - 1][z];
                }
                blockss[x + 0 * bianshu + z * bianshu_2] = blockss[blockshuliang];
                VAOBOX[x + 0 * bianshu + z * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[x + 0 * bianshu + z * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[x][0][z] = 0;
                shnegchen_Blockkong[x][0][z] = 0;
            }
        }
    }
    else if (int(camerapos.z / blockboxshu) - Blockposy == 1)
    {
        for (size_t x = 0; x < bianshu; x++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                blockss[blockshuliang] = blockss[x + 0 * bianshu + z * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[x + 0 * bianshu + z * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[x + 0 * bianshu + z * bianshu_2];
                //delete blocks[x + 0 * bianshu + z * bianshu_2];
                for (size_t y = 0; y < bianshu - 1; y++)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x + (y + 1) * bianshu + z * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x + (y + 1) * bianshu + z * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x + (y + 1) * bianshu + z * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x][y + 1][z];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x][y + 1][z];
                }
                blockss[x + (bianshu - 1) * bianshu + z * bianshu_2] = blockss[blockshuliang];
                VAOBOX[x + (bianshu - 1) * bianshu + z * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[x + (bianshu - 1) * bianshu + z * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[x][bianshu - 1][z] = 0;
                shnegchen_Blockkong[x][bianshu - 1][z] = 0;
            }
        }
    }


    if (int(camerapos.y / blockboxshu) - Blockposz == -1)
    {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t x = 0; x < bianshu; x++)
            {
                blockss[blockshuliang] = blockss[x + y * bianshu + (bianshu - 1) * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[x + y * bianshu + (bianshu - 1) * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[x + y * bianshu + (bianshu - 1) * bianshu_2];
                //delete blocks[x + y * bianshu + (bianshu - 1) * bianshu_2];
                for (size_t z = bianshu - 1; z > 0; z--)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x + y * bianshu + (z - 1) * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x + y * bianshu + (z - 1) * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x + y * bianshu + (z - 1) * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x][y][z - 1];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x][y][z - 1];
                }
                blockss[x + y * bianshu + 0 * bianshu_2] = blockss[blockshuliang];
                VAOBOX[x + y * bianshu + 0 * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[x + y * bianshu + 0 * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[x][y][0] = 0;
                shnegchen_Blockkong[x][y][0] = 0;
            }
        }
    }
    else if (int(camerapos.y / blockboxshu) - Blockposz == 1)
    {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t x = 0; x < bianshu; x++)
            {
                blockss[blockshuliang] = blockss[x + y * bianshu + 0 * bianshu_2];
                VAOBOX[blockshuliang] = VAOBOX[x + y * bianshu + 0 * bianshu_2];
                VBOBOX[blockshuliang] = VBOBOX[x + y * bianshu + 0 * bianshu_2];
                //delete blocks[x + y * bianshu + 0 * bianshu_2];
                for (size_t z = 0; z < bianshu - 1; z++)
                {
                    blockss[x + y * bianshu + z * bianshu_2] = blockss[x + y * bianshu + (z + 1) * bianshu_2];
                    VAOBOX[x + y * bianshu + z * bianshu_2] = VAOBOX[x + y * bianshu + (z + 1) * bianshu_2];
                    VBOBOX[x + y * bianshu + z * bianshu_2] = VBOBOX[x + y * bianshu + (z + 1) * bianshu_2];
                    Blockkong[x][y][z] = Blockkong[x][y][z + 1];
                    shnegchen_Blockkong[x][y][z] = shnegchen_Blockkong[x][y][z + 1];
                }
                blockss[x + y * bianshu + (bianshu - 1) * bianshu_2] = blockss[blockshuliang];
                VAOBOX[x + y * bianshu + (bianshu - 1) * bianshu_2] = VAOBOX[blockshuliang];
                VBOBOX[x + y * bianshu + (bianshu - 1) * bianshu_2] = VBOBOX[blockshuliang];
                Blockkong[x][y][bianshu - 1] = 0;
                shnegchen_Blockkong[x][y][bianshu - 1] = 0;
            }
        }
    }


    Blockposx = camerapos.x / blockboxshu;
    Blockposy = camerapos.z / blockboxshu;
    Blockposz = camerapos.y / blockboxshu;

    int gxshu = 0;
    std::vector<std::thread> Threads;
    for (size_t y = 0; y < bianshu; y++)
    {
        for (size_t x = 0; x < bianshu; x++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                if (Blockkong[x][y][z] == 0 && shnegchen_Blockkong[x][y][z] == 0)
                {
                    gxshu++;
                    Threads.push_back(std::thread(&block::blockinit, blockss[x + y * bianshu + z * bianshu_2], (x + Blockposx) * blockboxshu, (y + Blockposy) * blockboxshu, (z + Blockposz - pos_pianyi) * blockboxshu));
                    if (gxshu == threadshu) {
                        x = bianshu;
                        y = bianshu;
                        z = bianshu;
                    }
                }
            }
        }
    }
    
    for (int ix = 0; ix < gxshu; ix++)
    {
        Threads[ix].join();
    }

    for (size_t y = 0; y < bianshu; y++)
    {
        for (size_t x = 0; x < bianshu; x++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                if (Blockkong[x][y][z] == 0)
                {
                    gxshu--;
                    shnegchen_Blockkong[x][y][z] = 1;
                    if (gxshu == 0) {
                        x = bianshu;
                        y = bianshu;
                        z = bianshu;
                        shengchen_kai = true;
                    }
                }
            }
        }
    }
}


void blocks::Threadblockxinit(int x, int y, int z)
{
    blockss[x + y * bianshu + z * bianshu_2]->blockinit((x + Blockposx) * blockboxshu, (y + Blockposy) * blockboxshu, (z + Blockposz - pos_pianyi) * blockboxshu);
    glBindVertexArray(VAOBOX[x + y * bianshu + z * bianshu_2]);
    // 2. 把我们的顶点数组复制到一个顶点缓冲中，供OpenGL使用
    glBindBuffer(GL_ARRAY_BUFFER, VBOBOX[x + y * bianshu + z * bianshu_2]);
    glBufferData(GL_ARRAY_BUFFER, blockss[x + y * bianshu + z * bianshu_2]->DATA.vertexsizeof, blockss[x + y * bianshu + z * bianshu_2]->DATA.vertex, GL_STATIC_DRAW);
    delete[]blockss[x + y * bianshu + z * bianshu_2]->DATA.vertex;
    // 4. 设定顶点属性指针
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

//区块附近
void blocks::gengxinblock(int x, int y, int z, int blockshui, int px, int py, int pz)
{
    if (px < 0)x = blockss[blockshui]->jiaozhen(x);
    if (py < 0)y = blockss[blockshui]->jiaozhen(y);
    if (pz < 0)z = blockss[blockshui]->jiaozhen(z);
    printf("%d  %d  %d  %d\n", x, y, z, blockshui);
    if (x == 0)
    {
        blockss[blockshui - 1]->blockgengxin(blockboxshu, y, z);
        shuaxinblock(blockshui - 1);
    }
    if (z == 0)
    {
        blockss[blockshui - bianshu]->blockgengxin(x, y, blockboxshu);
        shuaxinblock(blockshui - bianshu);
    }
    if (y == 0)
    {
        blockss[blockshui - bianshu_2]->blockgengxin(x, blockboxshu, z);
        shuaxinblock(blockshui - bianshu_2);
    }
    if (x == blockboxshu-1)
    {
        blockss[blockshui + 1]->blockgengxin(-1, y, z);
        shuaxinblock(blockshui + 1);
    }
    if (z == blockboxshu-1)
    {
        blockss[blockshui + bianshu]->blockgengxin(x, y, -1);
        shuaxinblock(blockshui + bianshu);
    }
    if (y == blockboxshu-1)
    {
        blockss[blockshui + bianshu_2]->blockgengxin(x, -1, z);
        shuaxinblock(blockshui + bianshu_2);
    }
}

//区块附近y
void blocks::gengxinblocky(int x, int y, int z, int blockshui, int px, int py, int pz)
{
    if (px < 0)x = blockss[blockshui]->jiaozhen(x);
    if (py < 0)y = blockss[blockshui]->jiaozhen(y);
    if (pz < 0)z = blockss[blockshui]->jiaozhen(z);
    printf("%d  %d  %d  %d\n", x, y, z, blockshui);
    if (x == 0)
    {
        blockss[blockshui - 1]->blockgengxiny(blockboxshu, y, z, 13112341);
        shuaxinblock(blockshui - 1);
    }
    if (z == 0)
    {
        blockss[blockshui - bianshu]->blockgengxiny(x, y, blockboxshu, 13112341);
        shuaxinblock(blockshui - bianshu);
    }
    if (y == 0)
    {
        blockss[blockshui - bianshu_2]->blockgengxiny(x, blockboxshu, z, 13112341);
        shuaxinblock(blockshui - bianshu_2);
    }
    if (x == blockboxshu-1)
    {
        blockss[blockshui + 1]->blockgengxiny(-1, y, z, 13112341);
        shuaxinblock(blockshui + 1);
    }
    if (z == blockboxshu-1)
    {
        blockss[blockshui + bianshu]->blockgengxiny(x, y, -1, 13112341);
        shuaxinblock(blockshui + bianshu);
    }
    if (y == blockboxshu-1)
    {
        blockss[blockshui + bianshu_2]->blockgengxiny(x, -1, z, 13112341);
        shuaxinblock(blockshui + bianshu_2);
    }
}

//刷新区块
void blocks::shuaxinblock(int blockshui)
{
    glBindVertexArray(VAOBOX[blockshui]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOBOX[blockshui]);

    glBufferData(GL_ARRAY_BUFFER, blockss[blockshui]->DATA.vertexsizeof, blockss[blockshui]->DATA.vertex, GL_STATIC_DRAW);
    printf("%d", blockshui);
    delete[]blockss[blockshui]->DATA.vertex;
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
}

void blocks::shnegchen_block(Shader* ShaderB)
{
    ShaderB->use();
    //获取每一帧的鼠标移动状态（转动摄像机）
    glUniformMatrix4fv(glGetUniformLocation(ShaderB->ID, "view"), 1, GL_FALSE, glm::value_ptr(view));

    //平行光
    //Shader->SetUniform3f("lightD.pos", lightD.position);
    ShaderB->SetUniform3f("lightD.color", lightD.color);
    ShaderB->SetUniform3f("lightD.dirToLight", lightD.angles);
    /*
    //点光源
    Shader->SetUniform3f("lightP.pos", lightP.position);
    Shader->SetUniform3f("lightP.color", lightP.color);
    Shader->SetUniform3f("lightP.dirToLight", lightP.direction);
    Shader->SetUniform1f("lightP.constant", lightP.constant);
    Shader->SetUniform1f("lightP.linear", lightP.linear);
    Shader->SetUniform1f("lightP.quadratic", lightP.quadratic);
    */
    //聚光灯
    ShaderB->SetUniform3f("lightS.pos", camerapos);
    ShaderB->SetUniform3f("lightS.color", lightS.color);
    ShaderB->SetUniform3f("lightS.dirToLight", -cameraForward);
    ShaderB->SetUniform1f("lightS.constant", lightS.constant);
    ShaderB->SetUniform1f("lightS.linear", lightS.linear);
    ShaderB->SetUniform1f("lightS.quadratic", lightS.quadratic);
    ShaderB->SetUniform1f("lightS.CosPhyInner", lightS.cosPhyInner);
    ShaderB->SetUniform1f("lightS.CosPhyOutter", lightS.cosPhyOutter);

        
    ShaderB->SetUniform3f("cameraFos", cameraForward);


    /*
    int x1, y1, z1;
    for (size_t x = 0; x < bianshu; x++)
    {
        for (size_t y = 0; y < bianshu; y++)
        {
            for (size_t z = 0; z < bianshu; z++)
            {
                int lage = x + z * bianshu + y * bianshu_2;
                if (blockss[lage]->DATA.shu > 0 && Blockkong[x][z][y])
                {   
                    x1 = (x - pos_pianyi + int(camerapos.x / 64)) * 64;
                    y1 = (y - pos_pianyi + int(camerapos.y / 64)) * 64;
                    z1 = (z - pos_pianyi + int(camerapos.z / 64)) * 64;
                    glBindVertexArray(VAOBOX[lage]);
                    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x1, y1, z1));//在vec()中无法计算结果为负数的结果
                    glUniformMatrix4fv(glGetUniformLocation(ShaderB->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
                    glDrawArrays(GL_TRIANGLES, 0, blockss[lage]->DATA.shu);
                }
            }
        }
    }
    glBindVertexArray(0);*/
    
    
    
    for (size_t i = 0; i < blockshuliang; i++)
    {
        int x = i % bianshu;
        int y = i / bianshu_2;
        int z = (i / bianshu) % bianshu;

        if (blockss[i]->DATA.shu > 0 && Blockkong[x][z][y] && kai[i])
        {

            x = (x - pos_pianyi + int(camerapos.x / blockboxshu)) * blockboxshu;
            y = (y - pos_pianyi + int(camerapos.y / blockboxshu)) * blockboxshu;
            z = (z - pos_pianyi + int(camerapos.z / blockboxshu)) * blockboxshu;

            glBindVertexArray(VAOBOX[i]);
            glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(x, y, z));//在vec()中无法计算结果为负数的结果
            glUniformMatrix4fv(glGetUniformLocation(ShaderB->ID, "model"), 1, GL_FALSE, glm::value_ptr(model));
            glDrawArrays(GL_TRIANGLES, 0, blockss[i]->DATA.shu);
        }
    }
    
    glBindVertexArray(0);
}


void blocks::Threadblock_shnegchen_block(int a, int b) {
    for (int i = a; i < b; i++)
    {
        int x = i % bianshu;
        int y = i / bianshu_2;
        int z = (i / bianshu) % bianshu;

        if (abs(x - pos_pianyi) > 1 || abs(y - pos_pianyi) > 1 || abs(z - pos_pianyi) > 1) {

            int posx = 2;
            int posy = 2;
            int posz = 2;

            if (cameraForward.x < 0)posx = -posx;
            if (cameraForward.y < 0)posy = -posy;
            if (cameraForward.z < 0)posz = -posz;

            float vectorDot = cameraForward.x * (x - pos_pianyi + posx) + cameraForward.y * (y - pos_pianyi + posy) + cameraForward.z * (z - pos_pianyi + posz);
            float vectorMold1 = sqrt(pow(cameraForward.x, 2) + pow(cameraForward.y, 2) + pow(cameraForward.z, 2));
            float vectorMold2 = sqrt(pow((x - pos_pianyi + posx), 2) + pow((y - pos_pianyi + posy), 2) + pow((z - pos_pianyi + posz), 2));
            float cosAngle = acos((vectorDot) / (vectorMold1 * vectorMold2));
            float Angle = 180 / 3.141592653589793 * cosAngle;

            //std::mutex lock();
            if (Angle > 45) { kai[i] = false; }
            else { kai[i] = true; }
            //std::mutex unlock();
        }
    }
}

void blocks::Threadblock_shnegchen_block_kai() {
    double sjian = glfwGetTime();
    /*
    std::vector<std::thread> Threads;
    int shuliangbox = blockshuliang / threadshu;
    for (int ix = 0; ix < threadshu - 1; ix++)
    {
        Threads.push_back(std::thread(&blocks::Threadblock_shnegchen_block, this, ix * shuliangbox, (ix + 1) * shuliangbox));
    }
    Threads.push_back(std::thread(&blocks::Threadblock_shnegchen_block, this, (threadshu - 1) * shuliangbox, blockshuliang));
    for (int ix = 0; ix < threadshu; ix++)
    {
        Threads[ix].join();
    }*/
    Threadblock_shnegchen_block(0, blockshuliang);
    //std::cout << "剔除耗时  :   " << glfwGetTime() - sjian << std::endl;
}