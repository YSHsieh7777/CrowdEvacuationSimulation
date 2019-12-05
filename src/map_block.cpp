#include "simulator/map_block.hpp"

MapBlock::MapBlock(size_t left_bound, size_t right_bound, size_t up_bound, 
                        size_t bottom_bound, size_t x_people_num, size_t y_people_num,
						bool left_door, bool right_door, bool up_door, 
						bool bottom_door): 
                        m_left_bound(left_bound), m_right_bound(right_bound),
                        m_up_bound(up_bound), m_bottom_bound(bottom_bound),
						m_left_door(left_door), m_right_door(right_door),
						m_up_door(up_door), m_bottom_door(bottom_door) 
{
    m_people_num = x_people_num * y_people_num;

	for(uint8_t y=1; y<y_people_num+1; ++y)
	{
		for(uint8_t x=1; x<x_people_num+1; ++x)
		{	
			// init a person every 25 pixels
			float rand_x_pos = x*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
			float rand_y_pos = y*25 + (17.5 - 7.5) * rand() / (RAND_MAX + 1.0) + 7.5;
			
			Person *p = new Person(m_left_bound + rand_x_pos, m_up_bound + rand_y_pos, 7.5);
			m_people.push_back(p);
		}
	}	

	m_average_x = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;
	m_average_y = 0.4 * rand() / (RAND_MAX + 1.0) - 0.2;	
}

MapBlock::~MapBlock()
{
	for(size_t i=0; i<m_people_num; i++)
	{
		delete m_people[i];
	}
}

void MapBlock::add_neighbors(MapBlock* l_neighbor, MapBlock* r_neighbor, MapBlock* u_neighbor, MapBlock* d_neighbor)
{
	m_l_neighbor = l_neighbor;
	m_r_neighbor = r_neighbor;
	m_u_neighbor = u_neighbor;
	m_d_neighbor = d_neighbor;
}

const size_t & MapBlock::people_num() const { return m_people_num; }
size_t & MapBlock::people_num() { return m_people_num; }
const std::vector<Person* > & MapBlock::people() const { return m_people; }
std::vector<Person *> & MapBlock::people() { return m_people; }

void MapBlock::update_people_speed()
{	
	for(size_t i=0; i<m_people_num; ++i)
	{	
		if(m_people[i]->pass_door())
		{
			m_people[i]->x_speed() = m_people[i]->x_next_speed();
			m_people[i]->y_speed() = m_people[i]->y_next_speed();
		}	
		else 
		{
			m_people[i]->x_speed() = m_people[i]->x_next_speed() + 0.5 * m_average_x;
			m_people[i]->y_speed() = m_people[i]->y_next_speed() + 0.5 * m_average_y;
		}	

		m_average_x = 0.95 * m_average_x + 0.05 * m_people[i]->x_speed();
		m_average_y = 0.95 * m_average_y + 0.05 * m_people[i]->y_speed();
	}

	std::cout << "average x: " << m_average_x << std::endl;
	std::cout << "average y: " << m_average_y << std::endl;
}

void MapBlock::check_walls_collision()
{	
	std::vector<size_t> erase_id;

	for(size_t i=0; i<m_people_num; ++i)
	{
		float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
		float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();
        float person_radius = m_people[i]->r();
        float move_left_bound = move_x_pos - person_radius;
        float move_right_bound = move_x_pos + person_radius;
        float move_up_bound = move_y_pos - person_radius;
        float move_bottom_bound = move_y_pos + person_radius;

		if(m_left_door)
		{
			if(move_left_bound < m_left_bound && (move_up_bound < (m_up_bound+90) || move_bottom_bound > (m_bottom_bound-90)))
			{
				m_people[i]->x_speed() =  -(m_people[i]->x() - m_left_bound - person_radius);
				m_people[i]->x_next_speed() = 0.2;
			}
			else if(move_left_bound < m_left_bound && (move_up_bound >= (m_up_bound+90) || move_bottom_bound <= (m_bottom_bound-90)))
			{
				if(!m_people[i]->pass_door() && m_people[i]->x_speed() < 0)
				{
					m_people[i]->x_next_speed() = -0.5;
					m_people[i]->y_next_speed() = 0;
					m_people[i]->pass_door() = true;
				}
			}
		}
		else
		{
			if(move_left_bound < m_left_bound)
			{
				m_people[i]->x_speed() =  -(m_people[i]->x() - m_left_bound - person_radius);
				m_people[i]->x_next_speed() = 0.2;
			}
		}
		
		if(m_right_door)
		{	
			if(move_right_bound > m_right_bound && (move_up_bound < (m_up_bound+90) || move_bottom_bound > (m_bottom_bound-90)))
			{
				m_people[i]->x_speed() =  m_right_bound - m_people[i]->x() - person_radius;
				m_people[i]->x_next_speed() = -0.2;
			}
			else if(move_right_bound > m_right_bound && (move_up_bound >= (m_up_bound+90) || move_bottom_bound <= (m_bottom_bound-90)))
			{
				//std::cout << "left: " << i << " speed: " << m_people[i]->x_speed() << std::endl;
				if(!m_people[i]->pass_door() && m_people[i]->x_speed() > 0)
				{
					//std::cout << "left: " << i << " speed: " << m_people[i]->x_speed() << std::endl;
					m_people[i]->x_next_speed() = 0.5;
					m_people[i]->y_next_speed() = 0;
					m_people[i]->pass_door() = true;
				}
			}
		}
		else
		{
			if(move_right_bound > m_right_bound)
			{
				m_people[i]->x_speed() = m_right_bound - m_people[i]->x() - person_radius;
				m_people[i]->x_next_speed() = -0.2;
			}
		}
		
		if(m_up_door)
		{
			if(move_up_bound < m_up_bound && (move_left_bound < (m_left_bound+90) || move_right_bound > (m_right_bound-90)))
			{
				m_people[i]->y_speed() = -(m_people[i]->y() - m_up_bound - person_radius);
				m_people[i]->y_next_speed() = 0.2;
			}
			else if(move_up_bound < m_up_bound && (move_left_bound >= (m_left_bound+90) || move_right_bound <= (m_right_bound-90)))
			{
				if(!m_people[i]->pass_door() && m_people[i]->y_speed() < 0)
				{
					m_people[i]->y_next_speed() = -0.5;
					m_people[i]->x_next_speed() = 0;
					m_people[i]->pass_door() = true;
				}
			}
		}
		else
		{
			if(move_up_bound < m_up_bound)
			{
				m_people[i]->y_speed() = -(m_people[i]->y() - m_up_bound - person_radius);
				m_people[i]->y_next_speed() = 0.1;
			}
		}
		
		if(m_bottom_door)
		{
			if(move_bottom_bound > m_bottom_bound && (move_left_bound < (m_left_bound+90) || move_right_bound > (m_right_bound-90)))
			{
				m_people[i]->y_speed() = m_bottom_bound - m_people[i]->y() - person_radius;
				m_people[i]->y_next_speed() = -0.2;
			}
			else if(move_bottom_bound > m_bottom_bound && (move_left_bound >= (m_left_bound+90) || move_right_bound <= (m_right_bound-90)))
			{
				if(!m_people[i]->pass_door() && m_people[i]->y_speed() > 0)
				{
					m_people[i]->y_next_speed() = 0.5;
					m_people[i]->x_next_speed() = 0;
					m_people[i]->pass_door() = true;
				}
			}
		}
		else
		{
			if(move_bottom_bound > m_bottom_bound)
			{
				m_people[i]->y_speed() = m_bottom_bound - m_people[i]->y() - person_radius;
				m_people[i]->y_next_speed() = -0.2;
			}
		}

		if(m_r_neighbor != NULL)
		{
			if(move_right_bound > (m_right_bound+30))
			{
				m_people[i]->pass_door() = false;
				m_r_neighbor->people().push_back(m_people[i]);
				m_r_neighbor->people_num() += 1;
				erase_id.push_back(i);
			}
		}
		else
		{	
			if(move_right_bound > (m_right_bound+30))
			{
				m_people[i]->pass_door() = false;
				erase_id.push_back(i);
			}
		}
			
		if(m_l_neighbor != NULL)
		{
			if(move_left_bound < (m_left_bound-30))
			{
				m_people[i]->pass_door() = false;
				m_l_neighbor->people().push_back(m_people[i]);
				m_l_neighbor->people_num() += 1;
				erase_id.push_back(i);
			}
		}
		else
		{
			if(move_left_bound < (m_left_bound-30))
			{
				m_people[i]->pass_door() = false;
				erase_id.push_back(i);
			}
		}

		if(m_u_neighbor != NULL)
		{
			if(move_up_bound < (m_up_bound-30))
			{
				m_people[i]->pass_door() = false;
				m_u_neighbor->people().push_back(m_people[i]);
				m_u_neighbor->people_num() += 1;
				erase_id.push_back(i);
			}
		}
		else
		{
			if(move_up_bound < (m_up_bound-30))
			{
				m_people[i]->pass_door() = false;
				erase_id.push_back(i);
			}
		}

		if(m_d_neighbor != NULL)
		{
			if(move_bottom_bound > (m_bottom_bound+30))
			{
				m_people[i]->pass_door() = false;
				m_d_neighbor->people().push_back(m_people[i]);
				m_d_neighbor->people_num() += 1;
				erase_id.push_back(i);
			}
		}
		else
		{
			if(move_bottom_bound > (m_bottom_bound+30))
			{
				m_people[i]->pass_door() = false;
				erase_id.push_back(i);
			}
		}
	}

	if(!erase_id.empty())
	{
		for(size_t i=0; i<erase_id.size(); ++i)
		{
			m_people.erase(m_people.begin()+erase_id[i]);
			m_people_num -= 1;
		}
	}
}

void MapBlock::check_person_collision(size_t num)
{
	float cur_move_x_pos = m_people[num]->x() + m_people[num]->x_speed();
	float cur_move_y_pos = m_people[num]->y() + m_people[num]->y_speed();

	for(size_t i=0; i<m_people_num; ++i)
	{
		if(m_people[i]->pass_door())
			continue;

		float move_x_pos = m_people[i]->x() + m_people[i]->x_speed();
		float move_y_pos = m_people[i]->y() + m_people[i]->y_speed();

		if(i == num)
			continue;
		else
		{
			if((abs(cur_move_x_pos-move_x_pos) < 15) && (abs(cur_move_y_pos-move_y_pos) < 15))
			{
				m_people[num]->x_speed() = 0;
				m_people[num]->y_speed() = 0;
				m_people[i]->x_speed() = 0;
				m_people[i]->y_speed() = 0;
				m_people[num]->x_next_speed() = m_people[num]->x_next_speed() + (cur_move_x_pos-move_x_pos) * 0.01;
				m_people[num]->y_next_speed() = m_people[num]->y_next_speed() + (cur_move_y_pos-move_y_pos) * 0.01;
				m_people[i]->x_next_speed() = m_people[i]->x_next_speed() + (move_x_pos-cur_move_x_pos) * 0.01;
				m_people[i]->y_next_speed() = m_people[i]->y_next_speed() + (move_y_pos-cur_move_y_pos) * 0.01;
				check_person_collision(i);
				check_person_collision(num);
				break;
			}
		}	
	}
}

void MapBlock::check_people_collision()
{
	// std::cout << "people size: " << m_people.size() << '\n';
	for(size_t i=0; i<m_people_num; ++i)
	{
		if(!m_people[i]->pass_door())
		{
			// std::cout<< "num: " << i << '\n';
			check_person_collision(i);
		}
			
	}
}

void MapBlock::update_people()
{
	check_walls_collision();
	check_people_collision();
	
	size_t lu_size = m_people.size();
	
	for(size_t i=0; i<lu_size; ++i)
	{
		m_people[i]->move();
	}

	// for(size_t i=0; i<lu_size; ++i)
	// {
	// 	std::cout << "person " << i << "  x: " << m_people[i]->x() << "  y: " << m_people[i]->y() << "\n";
	// }
	
	update_people_speed();
}