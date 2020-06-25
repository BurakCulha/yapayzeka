#include<iostream>
#include"SFML\Graphics.hpp"
#include"SFML\Window.hpp"
#include"SFML\System.hpp"
#include<math.h>
#include<cstdlib>
#include<vector>
#include <cmath>
#include <time.h>
#include <algorithm>


using namespace sf;


class Bullet
{
public:
	Texture missileTex;
	Sprite missile;
	Vector2f currVelocity;
	float maxSpeed;

	Bullet(): currVelocity(0.f, 0.f), maxSpeed(15.f)
	{
		if (!this->missileTex.loadFromFile("missile.png")) {std::cout<< "Could not load missile.png!";}
        this->missile.setTexture(this->missileTex);
        this->missile.setOrigin(32,16);
	}
};


class Tank
{
public:
	Texture bodyTex;
	Sprite body;
	Texture turetTex;
	Sprite turet;

	Tank()
	{
	    if (!this->bodyTex.loadFromFile("tank1.png")) {std::cout<< "Could not load tank1.png!";}
        this->body.setTexture(this->bodyTex);
        this->body.setOrigin(115,60);
        this->body.setScale(Vector2f(0.75f, 0.75f));
        this->body.setRotation(0);
        this-> body.setPosition(150.f,300.f);

        if (!this->turetTex.loadFromFile("turet1.png"))  {std::cout<<"Could not load turet.png!";	}
        this->turet.setTexture(this->turetTex);
        this->turet.setOrigin(90,60);
        this->turet.setScale(Vector2f(0.75f, 0.75f));
        this->turet.setRotation(0);
        this->turet.setPosition(this->body.getPosition());
	}


void	forw()
	{
	    this->body.move(2.f,0.f);
	    this->turet.move(2.f,0.f);
	}

void	backw()
	{
	    this->body.move(-2.f,0.f);
	    this->turet.move(-2.f,0.f);
	}

void	upw()
	{
        this->body.move(0.f,-2.f);
	    this->turet.move(0.f,-2.f);
	}

void	downw()
	{
        this->body.move(0.f,2.f);
	    this->turet.move(0.f,2.f);
	}

void	turnR()//float tval)
	{
//	    this->turet.setRotation(tval);
        this->turet.rotate(1);
	}
	void	turnL()//float tval)
	{

        this->turet.rotate(-1);
	}

};


//-------------------------------------------
//*****************NEURAL NETWORK


class NET
{
public:
    int num_of_layers;
    int total_weight_count = 0;
    std::vector<int> num_of_neurons_in_layer;
//    std::vector<std::vector<std::vector<float>>> weights;
    std::vector<std::vector<float>> weights;
    std::vector<float>now_state;
    std::vector<float>next_state;
    std::vector<float>output_of_neuron;
    float  sigmoid(float x)
{
    return 1/(1+ exp(-x));
}
    NET(std::vector<int> sizes)
    {
    srand(time(NULL));
    this->num_of_layers = sizes.size();
    this->num_of_neurons_in_layer = sizes;
//    std::vector<float>weight_i_i_plus_one;

    for (int i =0; i< this->num_of_layers-1; i++)
    {
        std::vector<float>layer_i_i_plus_one;
        for (int j =0 ; j< this->num_of_neurons_in_layer[i]*this->num_of_neurons_in_layer[i+1]; j++)
        {
            layer_i_i_plus_one.push_back((float) rand() / RAND_MAX -0.5 );

        }
        this->total_weight_count += layer_i_i_plus_one.size();
        this->weights.push_back(layer_i_i_plus_one);
    }
}


    std::vector<float> feed_forward(std::vector<float> game_state)
    {
   this->now_state = game_state;

  //     for(int i = 0 ; this->weights.size(); i++)
    for(int i = 0 ; i< this->num_of_layers-1; i++)
    {
        std::vector<float>output_of_layer;

        for(int j = 0; j< this->num_of_neurons_in_layer[i+1]; j++)
        {
           float val =0;
            for(int k = 0; k<this->num_of_neurons_in_layer[i]; k++)
            {
                val +=  (float) this->now_state[k]*this->weights[i][(j+1)*(k+1)-1];             //now_state[k]*this->weight
                std::cout<<val<<"norna giren  agirlik deger\n";
            }
            this->next_state.push_back(     this->sigmoid(val)         );
        }
        this->now_state = this->next_state;
        this->next_state.erase(   this->next_state.begin(),this->next_state.end()      );
    }
    this->output_of_neuron =    this->now_state;
    return this->output_of_neuron;
}

};    // *********************ANN END


//*****************GENETIC

class DNA
{
public:
    std::vector<float> gene_array;
//    std::vector<std::vector<float>> neural_ag;
    int gene_size;
    float fitness;
    int kill;
    int sec;
    int shoot_num;//derece olarak degistir ki turet donsun


    void mutate()
    {
        srand(time(NULL));
        float prob = 0.070;
        for (int i =0 ; i< this->gene_size; i++)
        {
            if(  prob>= (float) rand() / RAND_MAX   )
            {
                gene_array[i] = (float) rand() / RAND_MAX ;
            }
        }
    }
//    void crossover();
    void  calc_fitness()
    {
        float fit  = 10*this->kill   +  this->sec   -   this->shoot_num;
        if (  fit <0  )
        {
            this->fitness = 0 ;

        }
        this->fitness =fit ;
    }

    DNA(int gene_size = 5)//, NET netw)
    {
        this->gene_size = gene_size;
        srand(time(NULL));
        //    int i;
        for (int  i = 0 ; i< gene_size ; i++ )
            this->gene_array.push_back((float) rand() / RAND_MAX );
    }

    void set_fit_params(int kill, int sec, int shoot_num)
    {
        this->kill = kill;
        this->sec = sec;
        this->shoot_num = shoot_num;
    }
};






 bool sortByFitness(const DNA &sol_ind, const DNA &sag_ind)
 {
    return sol_ind.fitness > sag_ind.fitness;
 }





//===================================
//---------------------------populasyon
class POPULASYON
{
public:
//    static int generation_count = 0;
    int num_of_ind = 0;
    std::vector<DNA> individuals;// networklerden olusmali ya da etwork weightlerini dna dizisine aktarmaliyim.

    POPULASYON(int num_of_ind)
    {
        this->num_of_ind = num_of_ind;
        for (int i =0 ; i < num_of_ind; i++)
        {
            this->individuals.push_back( DNA());
        }
    }
//    append_next_generation();
    DNA   cross_over(DNA ind1, DNA ind2)
    {
        srand(time(NULL));

        DNA child;
        int i =0;
        std::cout<<ind1.gene_array.size()<<"===total gen boyutu\n";
        child.gene_array.clear();  //erase(child.gene_array.begin(),child.gene_array.begin());
        for( i =0; i<(int) ind1.gene_array.size()/4 ;i++)
        {
            //        empty_gene_array.push_back(ind1.gene_array[i]);
            child.gene_array.push_back(ind1.gene_array[i]);
            std::cout<<i<<" =i\n";
        }
        int j = 0;
        for( i+j ; j<(int) ind1.gene_array.size()/4 ;j++)
        {
            child.gene_array.push_back(ind2.gene_array[i+j]);
        }
         std::cout<<j<<" =j\n";

        int k = 0;
        for( i+j+k ; k<(int) ind1.gene_array.size()/4 ;k++)
        {
            child.gene_array.push_back(ind2.gene_array[i+j+k]);
        }
         std::cout<<k<<" =k\n";

        int l = 0;
        for( i+j+k+l ; l<(int) ind1.gene_array.size()-(i+j+k) ;l++)
        {
            child.gene_array.push_back(ind2.gene_array[i+j+k+l]);
        }
        std::cout<<l<<" =l\n";

        std::cout<<i+j+k+l <<" =toplam indisler\n";
        std::cout<<"burasi crossover ici"<<child.gene_array.size();
        return child;

    }

    void elitist_selection()
    {
        std::vector<DNA>stronger_individuals;
        //   POPULASYON stronger_individuals;
        //    stronger_individuals.num_of_ind = 0;
        srand(time(NULL));
        for (int i =0; i< this->num_of_ind;i++)
        {//fitness parametreleri disarda set edilmeli.
            //        this->individuals[i].set_fit_params(   rand()%30, rand()%30, rand()%30    );// ben set ettim kaldiracagim
            this->individuals[i].calc_fitness();
            //        std::cout<<this->individuals[i].kill<<"kill miktari \n";
        }

       std::sort(this->individuals.begin(),this->individuals.end(),  sortByFitness  );

    //  std::cout<< " ilk5"<< this->individuals[0].fitness<<"\n"<<  this->individuals[1].fitness<<"\n"<<  this->individuals[2].fitness<<"\n"<<this->individuals[3].fitness<<"\n"<<this->individuals[4].fitness<<"\n";

        for(int i =0; i<5;i++)
        {
            stronger_individuals.push_back(this->individuals[i]);// en iyi 5 li sonraki nesilde.
            //    stronger_individuals.num_of_ind++;
            //    this->individuals[i]
        }


        for(int i =0; i<5;i++)
        {
            DNA child = this->cross_over(this->individuals[i],this->individuals[i+5]);
            stronger_individuals.push_back(child);// en iyi ikinci 5 li sonraki nesilde.
            //    stronger_individuals.num_of_ind++;
            //    this->individuals[i]
        }


        for(int i =5; i<15;i++)
        {
            this->individuals[i].mutate();
            stronger_individuals.push_back(this->individuals[i]);//5 15 arasi da mutasyona ugradi
            // artik nesil 20 kisi
            //    stronger_individuals.num_of_ind++;
            //    this->individuals[i]
        }

        this->individuals = stronger_individuals;



    }
};

//*******END OF GENETIC





int main()
{
	srand(time(NULL));

	RenderWindow window(VideoMode(800, 600), "THINKTANK!", Style::Default);
	window.setFramerateLimit(60);




Tank t1;


//Bullets
Bullet b1;
std::vector<Bullet> bullets;


//Enemy
	RectangleShape enemy;
	enemy.setFillColor(Color::Magenta);
	enemy.setSize(Vector2f(50.f, 50.f));

	std::vector<RectangleShape> enemies;



    Vector2f tankCenter;
    Vector2f turetCenter;
//	Vector2f mousePosWindow;
	Vector2f aimDir;
	Vector2f aimDirNorm;
	Vector2f  enemyPosit;


// int k = 0;


NET ag({4,8,6,3});
std::cout<<ag.total_weight_count<<"total weight count\n";



	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}


        turetCenter = Vector2f(t1.turet.getPosition());



        //Player
		if (Keyboard::isKeyPressed(Keyboard::A))
			t1.backw();
		if (Keyboard::isKeyPressed(Keyboard::D))
            t1.forw();
		if (Keyboard::isKeyPressed(Keyboard::W))
            t1.upw();
		if (Keyboard::isKeyPressed(Keyboard::S))
            t1.downw();

        if (t1.body.getPosition().x <= 0) //Left
				{t1.body.setPosition(0.f, t1.body.getPosition().y);
                t1.turet.setPosition(t1.body.getPosition());
				}

			if (t1.body.getPosition().x >= window.getSize().x - t1.body.getGlobalBounds().width) //Right
				{
				    t1.body.setPosition(window.getSize().x - t1.body.getGlobalBounds().width, t1.body.getPosition().y);
                    t1.turet.setPosition(t1.body.getPosition());
				}
			if (t1.body.getPosition().y <= 0) //Top
				{
				    t1.body.setPosition(t1.body.getPosition().x, 0.f);
                    t1.turet.setPosition(t1.body.getPosition());
                }
			if (t1.body.getPosition().y >= window.getSize().y - t1.body.getGlobalBounds().height) //Bottom
				{
				    t1.body.setPosition(t1.body.getPosition().x, window.getSize().y - t1.body.getGlobalBounds().height);
                    t1.turet.setPosition(t1.body.getPosition());
				}





            if( enemies.size() < 1)
		{
			enemy.setPosition(Vector2f(rand() % window.getSize().x/2 + window.getSize().x/2-100 , rand() % window.getSize().y));
			enemies.push_back(RectangleShape(enemy));

//			spawnCounter = 0;
		}
		 enemyPosit = enemies[0].getPosition();

        aimDir.x = enemyPosit.x - turetCenter.x;
        aimDir.y = enemyPosit.y - turetCenter.y;
        aimDirNorm.x = aimDir.x / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));
        aimDirNorm.y = aimDir.y / sqrt(pow(aimDir.x, 2) + pow(aimDir.y, 2));


        float PI = 3.14159265f;
        float deg = atan2(aimDirNorm.y, aimDirNorm.x) * 180 / PI;

        float turet_rot = t1.turet.getRotation();

    std::vector<float> game_state;
    game_state.push_back(enemyPosit.x);
    game_state.push_back(enemyPosit.y);
    game_state.push_back(deg);
    game_state.push_back(turet_rot);


//         neural input- ann will control



         std::vector<float>a_vec = ag.feed_forward(game_state);
        std::cout <<"\n"<< a_vec[0]<<"=ilk\n"<<"\n"<< a_vec[1]<<"=iki\n"<<"\n"<< a_vec[2]<<"=uc\n";
        if (a_vec[0]>a_vec[1])
        {
            t1.turnL();
        }
        else
        {
            t1.turnR();
        }
        if (a_vec[2]>0.5)
        {
            b1.missile.setPosition(turetCenter);
			b1.currVelocity = aimDirNorm * b1.maxSpeed;
			b1.missile.setRotation(deg);
			bullets.push_back(Bullet(b1));
        }



//         if (a_vec[0]>0.7)
//         {
//             std::cout <<"\n"<< a_vec[0]<<"yukari hareket\n";
//             t1.upw();
//         }else
//         {
//             std::cout <<"\n"<< a_vec[0]<<"asagi hareket\n";
//             t1.downw();
//         }
//         if (a_vec[1]>0.7)
//            {
//            b1.missile.setPosition(turetCenter);
//			b1.currVelocity = aimDirNorm * b1.maxSpeed;
//			b1.missile.setRotation(deg);
//			bullets.push_back(Bullet(b1));
//            }
         // neural output


		//Shooting
//		if (Mouse::isButtonPressed(Mouse::Left))
//		{
//			b1.missile.setPosition(turetCenter);
//			b1.currVelocity = aimDirNorm * b1.maxSpeed;
//			b1.missile.setRotation(deg);
//			bullets.push_back(Bullet(b1));
//		}
//t1.turnL();
//std::cout<<t1.turet.getRotation()<<"rotasyon aldim\n";
		for (size_t i = 0; i < bullets.size(); i++)
		{
			bullets[i].missile.move(bullets[i].currVelocity);

			//Out of bounds
			if (bullets[i].missile.getPosition().x < 0 || bullets[i].missile.getPosition().x > window.getSize().x
				|| bullets[i].missile.getPosition().y < 0 || bullets[i].missile.getPosition().y > window.getSize().y)
			{
				bullets.erase(bullets.begin() + i);

			}
			else
			{
				//Enemy collision
				for (size_t k = 0; k < enemies.size(); k++)
				{
					if (bullets[i].missile.getGlobalBounds().intersects(enemies[k].getGlobalBounds()))
					{
						bullets.erase(bullets.begin() + i);
						enemies.erase(enemies.begin() + k);
						break;
					}
				}
			}
		}






//DRAW
		window.clear();

    for (size_t i = 0; i < enemies.size(); i++)
		{
			window.draw(enemies[i]);
		}
		for (size_t i = 0; i < bullets.size(); i++)
		{
			window.draw(bullets[i].missile);
		}


		window.draw(t1.body);
		window.draw(t1.turet);
		window.display();
	}

	return 0;
}





