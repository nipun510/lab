class RangeModule {
public:
    using rangeContType = std::set<std::pair<int,int>>;
    using rangeIter = rangeContType::iterator;
    RangeModule() {
        
    }
    
    void addRange(int left, int right) {
        if (queryRange(left, right)) {
            return;
        }
        int first = left;
        int second = right - 1;
        auto overlappingRanges = getOverlappingRanges(left, right);
        if (!overlappingRanges.empty()) {
            if (overlappingRanges[0]->first < first) {
                first = overlappingRanges[0]->first;
            }
            if (overlappingRanges[overlappingRanges.size() - 1]->second > second) {
                second = overlappingRanges[overlappingRanges.size() - 1]->second;
            }
        }
      
        
        for (auto & rangeIt : overlappingRanges) {
            _ranges.erase(rangeIt);
        }
        
        auto insertedIt = _ranges.insert({first, second}).first;
        bool erasePrevIt = false;
        bool eraseNextIt = false;
        // cases in which there is no overlapping, still newly inserted rang can be merged
        // with left range or right range.
        auto prevIt = insertedIt == _ranges.begin() ? _ranges.end() : std::next(insertedIt, -1);
        if (prevIt != _ranges.end()) {
            if (prevIt->second + 1 == insertedIt->first) {
                first = prevIt->first;
                erasePrevIt = true;
                _ranges.erase(prevIt);
            }
        }
        auto nextIt = std::next(insertedIt,  1);
        if (nextIt != _ranges.end()) {
            if (nextIt->first == insertedIt->second + 1) {
                second = nextIt->second;
                eraseNextIt = true;
                _ranges.erase(nextIt);
            }
        }
        if (erasePrevIt || eraseNextIt) {
            _ranges.erase(insertedIt);
            _ranges.insert({first, second});
        }
    }
    
    bool queryRange(int left, int right) {
        auto overlappingRanges = getOverlappingRanges(left, right, 1);
        if (overlappingRanges.size() == 1 &&
            isContainedOverlapping(left, right, overlappingRanges[0]) {
              return true;
        }
        return false;
    }
    
    void removeRange(int left, int right) {
        auto overlappingRanges = getOverlappingRanges(left, right);
        std::vector<std::pair<int,int>> rangesToInsert;
        if (!overlappingRanges.empty()) {
            auto firstRangeIt = overlappingRanges[0];
            auto lastRangeIt = overlappingRanges[overlappingRanges.size() - 1];
            if (isLeftOverlapping(left, right, firstRangeIt)) {
                rangesToInsert.push_back({firstRangeIt->first, left - 1});
            }  
            if (isRightOverlapping(left, right, lastRangeIt)) {
                rangesToInsert.push_back({right, lastRangeIt->second});
            } 
            if (isContainedOverlapping(left, right, firstRangeIt)) {
                if (firstRangeIt->first < left) {
                  rangesToInsert.push_back({firstRangeIt->first, left - 1});
                }
                if (firstRangeIt->second > (right-1)) {
                  rangesToInsert.push_back({right, firstRangeIt->second});
                }
                
            }
        }
        
        for (auto & rangeIt : overlappingRanges) {
            _ranges.erase(rangeIt);
        }
        for (auto & item : rangesToInsert) {
            _ranges.insert(item);
        }
    }
private:
    void printRanges() const {
        for (auto & range : _ranges) {
            std::cout << "[" << range.first << ", " << range.second << "], ";
        }
        std::cout << std::endl;
    }
    bool isLeftOverlapping(int left, int right, rangeIter iter) const {
        return (iter->first < left && iter->second >= left &&
                iter->second < (right - 1));
    }
    bool isRightOverlapping(int left, int right, rangeIter iter) const {
        return (iter->first <= (right - 1) && iter->second > (right - 1) &&
               iter->first > left);
    }
    bool isContainedOverlapping(int left, int right, rangeIter iter) const {
        return (iter->first <= left && iter->second >= (right - 1));
    }
    bool isContainingOverlapping(int left, int right, rangeIter iter) const {
        return (iter->first >= left && iter->second <= (right - 1));
    }
    
    std::vector<rangeIter> getOverlappingRanges(int left, int right, int maxCount = INT_MAX) const {
        int cnt = 0;
        std::vector<rangeIter> overlappingRanges;
        auto it = _ranges.lower_bound({left, left});
        auto prevIt = it == _ranges.begin() ? _ranges.end() : std::next(it, -1);
        if (prevIt != _ranges.end()) {
            if (prevIt->second >= left) {
                ++cnt;
                overlappingRanges.push_back(prevIt);
            }
        }
        if (it->first == left && cnt < maxCount) {
            if (isContainedOverlapping(left, right, it)) {
                ++cnt;
                overlappingRanges.push_back(it);
                return overlappingRanges;
            }
        }
        auto currIt = it;
        while (currIt != _ranges.end() && cnt < maxCount) {
            if (isRightOverlapping(left, right, currIt)) {
                overlappingRanges.push_back(currIt);
                ++cnt;
                break;
            } else if (isContainingOverlapping(left, right, currIt)) {
                ++cnt;
                overlappingRanges.push_back(currIt);
            } else {
                break;
            }
            ++currIt;
        }
        return overlappingRanges;
    }

    std::set<std::pair<int,int>> _ranges;
};
