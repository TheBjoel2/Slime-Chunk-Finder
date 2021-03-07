#include <iostream>
#include <cmath>
#include <fstream>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

RenderWindow win;

class JavaRandom
{
private:
    //need for a randomizer
    uint64_t rseed;

    inline constexpr int32_t next(int32_t bits)
    {
        rseed = (rseed * 0x5DEECE66DLL + 0xBLL) & 0xFFFFFFFFFFFFLL;

        return int32_t((rseed) >> (48 - bits));
    }
public:
    inline constexpr int32_t nextInt(int32_t bound)
    {
        int32_t r = next(31);
        int32_t m = bound - 1;
        /*
        if ((bound & m) == 0)
        {
            r = int32_t((bound * (int64_t)(r)) >> 31);
        }
        else
        */
            for(int32_t u = r; u - (r = u % bound) + m < 0; u = next(31));

        return r;
    }

    inline constexpr void setSeed(int64_t seedIn)
    {
        rseed = (seedIn ^ 0x5DEECE66DLL) & 0xFFFFFFFFFFFFLL;
    }
};

class chunkPosStack
{
private:
    class chunkPos
    {
    public:
        int32_t x;
        int32_t z;
        chunkPos* p_next = nullptr;
    };

    chunkPos* head = nullptr;
    chunkPos* iter_ptr;
    chunkPos* aux;
public:
    chunkPosStack()
    {
        head = new chunkPos;
    }
    void addPos(int32_t x, int32_t z)
    {
        chunkPos* p_aux = head;
        while(p_aux->p_next != nullptr)
            p_aux = p_aux->p_next;

        p_aux->p_next = new chunkPos;
        p_aux->p_next->x = x;
        p_aux->p_next->z = z;
    }
    void resetIter()
    {
        iter_ptr = head->p_next;
    }
    bool getNextPos(int32_t& x, int32_t& z)
    {
        if(iter_ptr == nullptr)
            return false;

        x = iter_ptr->x;
        z = iter_ptr->z;
        iter_ptr = iter_ptr->p_next;

        return true;
    }
    ~chunkPosStack()
    {
        iter_ptr = head->p_next;
        delete head;
        while(iter_ptr != nullptr)
        {
            aux = iter_ptr;
            iter_ptr = iter_ptr->p_next;
            delete aux;
        }
    }
};

class chunkFinder
{
private:
    int32_t x1;
    int32_t x2;
    int32_t z1;
    int32_t z2;
    int64_t seed;
    bool* isDone;
    int32_t* z_current;
    chunkPosStack s;

    //Rand
    JavaRandom rand;
public:
    void setup(int32_t x1, int32_t x2, int32_t z1, int32_t z2, int64_t seed, bool* isDone, int32_t* z_curr)
    {
        this->x1 = x1;
        this->x2 = x2;
        this->z1 = z1;
        this->z2 = z2;
        this->seed = seed;
        this->isDone = isDone;
        this->z_current = z_curr;
    }
    void startBruteForce()
    {
        int32_t chunkSum;
        int32_t biggest = 0;

        for(uint32_t z = z1; int32_t(z) <= z2; z++)
        {
            *z_current = z;
            //resetting sum
            chunkSum = 0;

            //pre generating chunkSum
            for(int32_t j = -7; j <= 7; j++)
                for(int32_t i = -7; i <= 7; i++)
                {
                    rand.setSeed(seed + (int64_t)(int32_t)((x1+i) * (x1+i) * 4987142U) + (int64_t)(int32_t)((x1+i) * 5947611U) + (int64_t)(int32_t)((z+j) * (z+j)) * 4392871LL + (int64_t)(int32_t)((z+j) * 389711U) ^ 987234911LL);
                    chunkSum+=rand.nextInt(10) == 0;
                }

            //quick check #1
            if(chunkSum >= biggest)
            {
                biggest = chunkSum;
                s.addPos(x1, z);
            }

            //weird calculations
            for(uint32_t x = x1; int32_t(x) <= x2; x++)
            {
                //substructing previous chunks
                for(int32_t j = -7; j <= 7; j++)
                {
                    rand.setSeed(seed + (int64_t)(int32_t)((x-7) * (x-7) * 4987142U) + (int64_t)(int32_t)((x-7) * 5947611U) + (int64_t)(int32_t)((z+j) * (z+j)) * 4392871LL + (int64_t)(int32_t)((z+j) * 389711U) ^ 987234911LL);
                    chunkSum-=rand.nextInt(10) == 0;
                }

                //adding next chunks
                for(int32_t j = -7; j <= 7; j++)
                {
                    rand.setSeed(seed + (int64_t)(int32_t)((x+8) * (x+8) * 4987142U) + (int64_t)(int32_t)((x+8) * 5947611U) + (int64_t)(int32_t)((z+j) * (z+j)) * 4392871LL + (int64_t)(int32_t)((z+j) * 389711U) ^ 987234911LL);
                    chunkSum+=rand.nextInt(10) == 0;
                }

                //quick check
                if(chunkSum >= biggest)
                {
                    biggest = chunkSum;
                    s.addPos(x+1, z);
                }
            }
        }
        *isDone = true;
    }
    chunkPosStack* getChunkPosStack()
    {
        return &s;
    }
};

class loadingScreen
{
private:
    int32_t* ptr;
    int16_t length;
    int32_t dur;
public:
    void setup(int32_t* ptr, int16_t length, int32_t dur)
    {
        this->ptr = ptr;
        this->length = length;
        this->dur = dur;
    }
    void draw()
    {
        while(true)
        {
            int32_t n = 0;
            for(int16_t i = 0; i < length; i++)
                n+=(ptr[i] + dur/length);
            system("cls");
            cout << "Bruteforcing... Please wait" << endl <<  100.0D*n/(2*dur) << "% is done" << endl << endl;
            for(int16_t i = 0; i < length; i++)
                cout << "Thread" << i << " is on z = " << ptr[i] << endl;
            sleep(milliseconds(5000));
        }
    }
};

class chunkFeild
{
private:
    class chunk
    {
    private:
        RectangleShape rect;
        JavaRandom rand;
    public:
        void setPositionOnScreen(int32_t x, int32_t y)
        {
            rect.setSize(Vector2f(25, 25));
            rect.setPosition(x*25, y*25);
        }
        void calculateColor(int32_t x, int32_t z, int64_t seed, bool flag)
        {
            rand.setSeed(seed + (int64_t)(int32_t)((x) * (x) * 4987142U) + (int64_t)(int32_t)((x) * 5947611U) + (int64_t)(int32_t)((z) * (z)) * 4392871LL + (int64_t)(int32_t)((z) * 389711U) ^ 987234911LL);
            if(rand.nextInt(10) == 0)
                if(flag)
                    rect.setFillColor(Color(50, 255, 50));
                else
                    rect.setFillColor(Color(255, 75, 75));
            else
                rect.setFillColor(Color::White);

        }
        void draw()
        {
            win.draw(rect);
        }
    };
    chunk field[25][25];
public:
    void setupFeild(int32_t x, int32_t z, int64_t seed)
    {
        for(int32_t j = -12; j <= 12; j++)
            for(int32_t i = -12; i <= 12; i++)
            {
                field[12+i][12+j].setPositionOnScreen(12+i, 12+j);
                field[12+i][12+j].calculateColor(x+i, z+j, seed, sqrt(i*i*256+j*j*256) < 128);
                field[12+i][12+j].draw();
            }
    }
};

int32_t countUsableSlimeChunks(int32_t x, int32_t z, int64_t seed)
{
    int32_t toReturn = 0;
    JavaRandom rand;
    for(int32_t j = -7; j <= 7; j++)
        for(int32_t i = -7; i <= 7; i++)
        {
            rand.setSeed(seed + (int64_t)(int32_t)((x+i) * (x+i) * 4987142U) + (int64_t)(int32_t)((x+i) * 5947611U) + (int64_t)(int32_t)((z+j) * (z+j)) * 4392871LL + (int64_t)(int32_t)((z+j) * 389711U) ^ 987234911LL);
            toReturn+=rand.nextInt(10) == 0 && sqrt(i*i*256+j*j*256) < 128;
        }
    return toReturn;
}

void drawChunkPattern(int32_t x, int32_t z, int64_t seed)
{
    JavaRandom rand;
    for(int32_t j = -7; j <= 7; j++)
    {
        for(int32_t i = -7; i <= 7; i++)
        {
            rand.setSeed(seed + (int64_t)(int32_t)((x+i) * (x+i) * 4987142U) + (int64_t)(int32_t)((x+i) * 5947611U) + (int64_t)(int32_t)((z+j) * (z+j)) * 4392871LL + (int64_t)(int32_t)((z+j) * 389711U) ^ 987234911LL);
            if(rand.nextInt(10) == 0 && sqrt(i*i*256+j*j*256) < 128)
                cout << 'S' ;
            else
                cout << ' ' ;
        }
        cout << endl;
    }
}

int main()
{
    int64_t seed;
    cout << "Enter seed" << endl;
    cin >> seed;

    int32_t dur;
    cout << "Enter duration" << endl;
    cin >> dur;

    int16_t thread_num;
    cout << "Enter thread num" << endl;
    cin >> thread_num;

    int32_t* z_curr_array = new int32_t[thread_num];
    for(int16_t i = 0; i < thread_num; i++)
        z_curr_array[i] = 0;
    chunkFinder* cf = new chunkFinder[thread_num];
    Thread** thread_array = new Thread*[thread_num-1];
    Thread* aux;
    int32_t dur_aux = dur;
    int32_t d = (dur*2+1)/thread_num;
    bool* isThreadDone = new bool[thread_num-1];
    loadingScreen ls;
    ls.setup(z_curr_array, thread_num, dur);
    Thread loadingScreen(&loadingScreen::draw, &ls);
    for(int16_t i = 0; i < thread_num-1; i++)
    {
        isThreadDone[i] = false;
    }

    for(int16_t i = 0; i < thread_num-1; i++)
    {
        cf[i].setup(-dur, dur, -dur_aux, -dur_aux+d, seed, &(isThreadDone[i]), &(z_curr_array[i]));
        aux = new Thread(&chunkFinder::startBruteForce, &cf[i]);
        thread_array[i] = aux;
        dur_aux-=d+1;
    }
    cf[thread_num-1].setup(-dur, dur, -dur_aux, dur, seed, &(isThreadDone[thread_num-1]), &(z_curr_array[thread_num-1]));

    for(int16_t i = 0; i < thread_num-1; i++)
    {
        aux = thread_array[i];
        aux->launch();
    }

    loadingScreen.launch();
    cf[thread_num-1].startBruteForce();

    for(int16_t i = 0; i < thread_num-1; i++)
    {
        while(!isThreadDone[i])
            sleep(milliseconds(100));
    }
    loadingScreen.terminate();
    sleep(milliseconds(10));

    //finally analyzing data
    chunkPosStack finalStack;
    chunkPosStack* cps;
    int32_t biggest = 0;
    int32_t x_aux;
    int32_t z_aux;
    int32_t n_aux;
    //final chunkpos
    int32_t xf;
    int32_t zf;

    for(int16_t i = 0; i < thread_num; i++)
    {
        cps = cf[i].getChunkPosStack();
        cps->resetIter();
        while(cps->getNextPos(x_aux, z_aux))
        {
            n_aux = countUsableSlimeChunks(x_aux, z_aux, seed);
            bool flag = (n_aux == biggest && abs(x_aux)+abs(z_aux) < abs(xf)+abs(zf));
            if(n_aux > biggest || flag)
            {
                biggest = n_aux;
                xf = x_aux;
                zf = z_aux;
            }
        }
    }

    //console out
    system("cls");
    cout << endl;
    drawChunkPattern(xf, zf, seed);
    cout << endl;
    cout << "Found spot at " << xf << " " << zf << endl;
    cout << xf*16 << " " << zf*16 << " (pos in blocks)" << endl;
    cout << countUsableSlimeChunks(xf, zf, seed) << " chunks in total" << endl;
    cout << (abs(xf*16)+abs(zf*16)) << " blocks from 0 0 (lineal distance)" << endl;

    //write to a file
    JavaRandom rand;
    ofstream o;
    o.open("ChunkPos.txt");
    for(int32_t j = -7; j <= 7; j++)
    {
        for(int32_t i = -7; i <= 7; i++)
        {
            rand.setSeed(seed + (int64_t)(int32_t)((xf+i) * (xf+i) * 4987142U) + (int64_t)(int32_t)((xf+i) * 5947611U) + (int64_t)(int32_t)((zf+j) * (zf+j)) * 4392871LL + (int64_t)(int32_t)((zf+j) * 389711U) ^ 987234911LL);
            if(rand.nextInt(10) == 0 && sqrt(i*i*256+j*j*256) < 128)
                o << "S";
            else
                o << " ";
        }
        o << endl;
    }
    o << "Seed: " << seed << endl;
    o << "Duration: " << dur << endl;
    o << endl;
    o << "Found spot at " << xf << " " << zf << endl;
    o << xf*16 << " " << zf*16 << " (pos in blocks)" << endl;
    o << countUsableSlimeChunks(xf, zf, seed) << " chunks in total" << endl;
    o << (abs(xf*16)+abs(zf*16)) << " blocks from 0 0 (lineal distance)" << endl;
    o.close();

    //deleting
    for(int16_t i = 0; i < thread_num-1; i++)
    {
        aux = thread_array[i];
        delete aux;
    }
    delete[] thread_array;
    delete[] z_curr_array;
    delete[] isThreadDone;
    delete[] cf;

    //win out
    Event e;
    e.type = Event::Count;
    win.create(VideoMode(25*25, 25*25), "Chunk Pattern");
    win.pollEvent(e);
    sleep(milliseconds(1000));
    chunkFeild cfield;
    win.clear();
    cfield.setupFeild(xf, zf, seed);
    win.display();

    while(e.type != Event::Closed)
    {
        win.pollEvent(e);
        sleep(milliseconds(50));
    }
    win.close();

    return 0;
}
