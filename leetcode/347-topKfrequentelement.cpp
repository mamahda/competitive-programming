class Solution {
public:
    vector<int> topKFrequent(vector<int>& nums, int k) {
        unordered_map<int, int> frequency{};
        for (auto num : nums) {
            frequency[num]++;
        }

        priority_queue<pair<int, int>> mostk;
        for(auto freq : frequency) {
            mostk.push(make_pair(freq.second, freq.first));
        }

        vector<int> res;
        while (k--) {
            res.push_back(mostk.top().second);
            mostk.pop();
        }

        return res;
    }
};
