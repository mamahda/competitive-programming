#include <cstdio>
#include <cstring>

#define MAXN 105
#define MAXLABEL 205
#define MAXPERIM 405

struct Port
{
    int label;
    int r, c;
    int dir;
    int idx;
};

int X, Y;

int topLabel[MAXN], bottomLabel[MAXN];
int leftLabel[MAXN], rightLabel[MAXN];

char grid[MAXN][MAXN];

Port perimeter[MAXPERIM];
int perimSize;

Port endpoints[MAXLABEL][2];
int endpointCount[MAXLABEL];

int dr[4] = {-1, 0, 1, 0};
int dc[4] = {0, 1, 0, -1};

int turnDir(int dir, char mirror)
{
    if (mirror == '/')
    {
        if (dir == 0)
            return 1;
        if (dir == 1)
            return 0;
        if (dir == 2)
            return 3;
        return 2;
    }
    else
    {
        if (dir == 0)
            return 3;
        if (dir == 3)
            return 0;
        if (dir == 1)
            return 2;
        return 1;
    }
}

char mirrorForRightTurn(int dir)
{
    if (dir == 0 || dir == 2)
        return '/';
    return '\\';
}

int getExitLabel(int r, int c)
{
    if (r < 0)
        return topLabel[c];
    if (r >= X)
        return bottomLabel[c];
    if (c < 0)
        return leftLabel[r];
    return rightLabel[r];
}

void addPort(int label, int r, int c, int dir)
{
    Port p;
    p.label = label;
    p.r = r;
    p.c = c;
    p.dir = dir;
    p.idx = perimSize;
    perimeter[perimSize++] = p;

    int cnt = endpointCount[label];
    endpoints[label][cnt] = p;
    endpointCount[label]++;
}

int checkNonCrossing()
{
    int stack[MAXPERIM];
    int stackTop = 0;
    int seen[MAXLABEL];
    memset(seen, 0, sizeof(seen));

    for (int i = 0; i < perimSize; i++)
    {
        int v = perimeter[i].label;
        if (!seen[v])
        {
            seen[v] = 1;
            stack[stackTop++] = v;
        }
        else
        {
            if (stackTop == 0 || stack[stackTop - 1] != v)
            {
                return 0;
            }
            stackTop--;
        }
    }
    return (stackTop == 0);
}

int traceFrom(Port start)
{
    int label = start.label;
    int r = start.r + dr[start.dir];
    int c = start.c + dc[start.dir];
    int dir = start.dir;

    int steps = 0;
    int maxSteps = 4 * X * Y + 10;

    while (r >= 0 && r < X && c >= 0 && c < Y)
    {
        if (++steps > maxSteps)
            return 0;

        if (grid[r][c] == '.')
        {
            grid[r][c] = mirrorForRightTurn(dir);
        }
        dir = turnDir(dir, grid[r][c]);
        r += dr[dir];
        c += dc[dir];
    }

    return (getExitLabel(r, c) == label);
}

int verifyFinal()
{
    for (int i = 0; i < perimSize; i++)
    {
        int r = perimeter[i].r + dr[perimeter[i].dir];
        int c = perimeter[i].c + dc[perimeter[i].dir];
        int dir = perimeter[i].dir;

        int steps = 0;
        int maxSteps = 4 * X * Y + 10;

        while (r >= 0 && r < X && c >= 0 && c < Y)
        {
            if (++steps > maxSteps)
                return 0;
            dir = turnDir(dir, grid[r][c]);
            r += dr[dir];
            c += dc[dir];
        }

        if (getExitLabel(r, c) != perimeter[i].label)
            return 0;
    }
    return 1;
}

struct OrderItem
{
    int idx;
    int label;
};

void insertionSort(OrderItem arr[], int n)
{
    for (int i = 1; i < n; i++)
    {
        OrderItem key = arr[i];
        int j = i - 1;
        while (j >= 0 && arr[j].idx > key.idx)
        {
            arr[j + 1] = arr[j];
            j--;
        }
        arr[j + 1] = key;
    }
}

int main()
{
    scanf("%d %d", &X, &Y);

    memset(endpointCount, 0, sizeof(endpointCount));
    perimSize = 0;

    for (int j = 0; j < Y; j++)
        scanf("%d", &topLabel[j]);
    for (int i = 0; i < X; i++)
        scanf("%d %d", &leftLabel[i], &rightLabel[i]);
    for (int j = 0; j < Y; j++)
        scanf("%d", &bottomLabel[j]);

    for (int j = 0; j < Y; j++)
        addPort(topLabel[j], -1, j, 2);
    for (int i = 0; i < X; i++)
        addPort(rightLabel[i], i, Y, 3);
    for (int j = Y - 1; j >= 0; j--)
        addPort(bottomLabel[j], X, j, 0);
    for (int i = X - 1; i >= 0; i--)
        addPort(leftLabel[i], i, -1, 1);

    int possible = 1;
    for (int v = 1; v <= X + Y; v++)
    {
        if (endpointCount[v] != 2)
        {
            possible = 0;
            break;
        }
    }

    if (possible && !checkNonCrossing())
    {
        possible = 0;
    }

    if (!possible)
    {
        printf("-1\n");
        return 0;
    }

    for (int i = 0; i < X; i++)
    {
        for (int j = 0; j < Y; j++)
            grid[i][j] = '.';
        grid[i][Y] = '\0';
    }

    OrderItem order[MAXLABEL];
    int orderSize = 0;

    for (int v = 1; v <= X + Y; v++)
    {
        order[orderSize].idx = endpoints[v][1].idx;
        order[orderSize].label = v;
        orderSize++;
    }

    insertionSort(order, orderSize);

    for (int i = 0; i < orderSize && possible; i++)
    {
        int v = order[i].label;
        if (!traceFrom(endpoints[v][1]))
        {
            possible = 0;
        }
    }

    if (!possible)
    {
        printf("-1\n");
        return 0;
    }

    for (int i = 0; i < X; i++)
        for (int j = 0; j < Y; j++)
            if (grid[i][j] == '.')
                grid[i][j] = '/';

    if (!verifyFinal())
    {
        printf("-1\n");
        return 0;
    }

    for (int i = 0; i < X; i++)
        printf("%s\n", grid[i]);

    return 0;
}
