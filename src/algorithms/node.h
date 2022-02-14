struct Node
{
    int x, y;
    int weight;
    int startToCurrent;

    Node *parent;

    int foodLeft;

    Node(int x, int y, int foodLeft, Node *parent)
    {
        this->x = x;
        this->y = y;
        this->weight = 0;
        this->foodLeft = foodLeft;
        this->parent = parent;
    }
    Node(int x, int y, int weight, int startToCurrent, int foodLeft, Node *parent)
    {
        this->x = x;
        this->y = y;
        this->weight = weight;
        this->foodLeft = foodLeft;
        this->startToCurrent = startToCurrent;
        this->parent = parent;
    }

    bool operator==(const Node &ob) const
    {
        return x == ob.x && y == ob.y && foodLeft == ob.foodLeft && weight == ob.weight;
    }

    bool operator<(const Node &ob) const
    {
        return x < ob.x || (x == ob.x && y < ob.y);
    }
};