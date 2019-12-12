class Door
{
public:
    Door(float , float , float , float , bool );
    ~Door() = default;

    const float & l_bound() const;
    const float & r_bound() const;
    const float & u_bound() const;
    const float & d_bound() const;
    const bool & go_outside() const;

private:
    float m_l_bound, m_r_bound, m_u_bound, m_d_bound;
    bool m_go_outside;
};