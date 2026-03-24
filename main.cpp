#include <bits/stdc++.h>
using namespace std;
using U128 = __int128;
struct Block
{
    vector<pair<int, int>> cells;
    Block rotate() const
    {
        Block ret;
        ret.cells.reserve(cells.size());
        for (auto [x, y] : cells)
            ret.cells.emplace_back(y, -x);
        int minx = INT_MAX, miny = INT_MAX;
        for (auto [x, y] : ret.cells)
        {
            minx = min(minx, x);
            miny = min(miny, y);
        }
        for (auto &[x, y] : ret.cells)
        {
            x -= minx;
            y -= miny;
        }
        return ret;
    }
    Block flip() const
    {
        Block ret;
        ret.cells.reserve(cells.size());
        for (auto [x, y] : cells)
            ret.cells.emplace_back(-x, y);
        int minx = INT_MAX, miny = INT_MAX;
        for (auto [x, y] : ret.cells)
        {
            minx = min(minx, x);
            miny = min(miny, y);
        }
        for (auto &[x, y] : ret.cells)
        {
            x -= minx;
            y -= miny;
        }
        return ret;
    }
    operator U128() const
    {
        U128 ret = 0;
        for (auto [x, y] : cells)
        {
            ret |= (U128)1 << (x * 10 + y);
        }
        return ret;
    }
};
int main()
{
    auto st = clock();
    vector<Block> base = {
        {{{0, 0}, {0, 1}, {1, 0}, {1, 1}, {2, 0}}},
        {{{2, 0}, {1, 0}, {1, 1}, {0, 1}, {0, 2}}},
        {{{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2}}},
        {{{0, 0}, {1, 0}, {0, 1}}},
        {{{0, 0}, {1, 0}, {2, 0}, {2, 1}}},
        {{{1, 0}, {1, 1}, {0, 1}, {2, 1}, {1, 2}}},
        {{{0, 0}, {1, 0}, {0, 1}, {1, 1}}},
        {{{0, 0}, {1, 0}, {0, 1}, {0, 2}, {1, 2}}},
        {{{0, 0}, {1, 0}, {1, 1}, {1, 2}, {1, 3}}},
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}}},
        {{{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 2}}},
        {{{0, 1}, {1, 1}, {2, 1}, {2, 0}, {3, 0}}}};
    U128 boardMask = 0;
    for (int r = 0; r < 10; r++)
        for (int c = 0; c < 10; c++)
            if (r + c < 10)
                boardMask |= (U128)1 << (r * 10 + c);
    vector<U128> orientations[12];
    for (int i = 0; i < 12; i++)
    {
        vector<U128> pool;
        Block cur = base[i];
        for (int k = 0; k < 4; k++)
        {
            pool.push_back((U128)cur);
            cur = cur.rotate();
        }
        cur = base[i].flip();
        for (int k = 0; k < 4; k++)
        {
            pool.push_back((U128)cur);
            cur = cur.rotate();
        }
        sort(pool.begin(), pool.end());
        pool.erase(unique(pool.begin(), pool.end()), pool.end());
        orientations[i] = move(pool);
    }
    static vector<U128> placements[12][100];
    for (int i = 0; i < 12; i++)
    {
        for (auto p : orientations[i])
        {
            int mx = 0;
            for (int i = 0; i < 100; i++)
                if (p >> i & 1)
                    mx = max(mx, i % 10);
            for (int shift = 0; shift < 100; shift++)
            {
                if (shift % 10 + mx >= 10)
                    continue;
                U128 placement = p << shift;
                if ((placement & ~boardMask) != 0)
                    continue;
                for (int pos = 0; pos < 100; pos++)
                    if ((placement >> pos) & 1)
                        placements[i][pos].push_back(placement);
            }
        }
        for (int pos = 0; pos < 100; pos++)
        {
            auto &vec = placements[i][pos];
            sort(vec.begin(), vec.end());
            vec.erase(unique(vec.begin(), vec.end()), vec.end());
        }
    }
    static unordered_map<U128, int> dp[101][1 << 12];
    dp[0][0].reserve(64);
    dp[0][0][0] = 1;
    U128 filledUpto = 0;
    for (int i = 0; i < 100; i++)
    {
        if (i > 0 && (boardMask >> (i - 1) & 1))
        {
            filledUpto |= (U128)1 << (i - 1);
        }
        for (int used = 0; used < (1 << 12); used++)
        {
            auto &mapAt = dp[i][used];
            if (mapAt.empty())
                continue;
            for (int k = 0; k < 12; k++)
            {
                if (used & (1 << k))
                    continue;
                int usedNext = used | (1 << k);
                auto &nextMap = dp[i][usedNext];
                if (nextMap.empty())
                    nextMap.reserve(mapAt.size() * 4);
                auto &posList = placements[k][i];
                if (posList.empty())
                    continue;
                for (auto placement : posList)
                {
                    if (placement & filledUpto)
                        continue;
                    for (auto &[mask, val] : mapAt)
                    {
                        if (placement & mask)
                            continue;
                        U128 merged = mask | placement;
                        int j = i;
                        while (j < 100 && (((~boardMask) | merged) >> j & 1))
                            j++;
                        dp[j][usedNext][merged] += val;
                    }
                }
            }
        }
        if (boardMask >> i & 1)
            filledUpto |= (U128)1 << i;
    }
    cout << dp[100][(1 << 12) - 1][boardMask] << '\n';
    cout << 1. * (clock() - st) / CLOCKS_PER_SEC << endl;
    return 0;
}
