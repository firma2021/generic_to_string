struct Zoo
{
private:
    int i;
    int j;

public:
    Zoo(int i, int j):
    i(i), j(j) { }
    friend ostream& operator<<(ostream& os, const Zoo& zoo)
    {
        os << "zoo: " << zoo.i << ", " << zoo.j << '\n';
        return os;
    }
};
