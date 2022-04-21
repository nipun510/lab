#include <vector>
#include <limits>
#include <string>
#include <map>
#include <type_traits>

#TODO make it generic and move to a separate file
int 
editDistance(const std::string &source, const std::string &target) {
  const int intmax = std::numeric_limits<int>::max();
  std::vector<std::vector<int>> distance(source.length() + 1, std::vector(target.length() + 1, intmax));
  distance[0][0] = 0;

  for (int j = 1; j <= target.size(); ++j) {
    distance[0][j] = distance[0][j-1] + 1;
  }

  for (int i = 1; i <= source.size(); ++i) {
    distance[i][0] = distance[i-1][0] + 1;
  }

  for (int i = 1; i <= source.length(); ++i) {
    for (int j = 1; j <= target.length(); ++j) {
      distance[i][j] = source[i-1] == target[j-1] ?  distance[i-1][j-1] : distance[i-1][j-1] + 1;
      distance[i][j] = std::min(distance[i][j], distance[i-1][j] + 1);
      distance[i][j] = std::min(distance[i][j], distance[i][j-1] + 1);
    }
  }

  return distance[source.length()][target.length()];
}

enum class lineChangeType {
  same,
  change,
  add,
  del
};



std::ostream & operator << (std::ostream &os, const lineChangeType type) {
  int val = static_cast<std::underlying_type_t<lineChangeType>>(type);
  os << std::to_string(val);
  return os;
}



std::vector<std::pair<int, lineChangeType>> 
filesDiff(const std::vector<std::string> &file1Lines, 
         const std::vector<std::string> &file2Lines){
  auto n = file1Lines.size();
  auto m = file2Lines.size();
  const int intmax = std::numeric_limits<int>::max();
  std::vector<std::vector<unsigned>> diffCount(n+1, std::vector<unsigned>(m+1, intmax));
  std::map<std::pair<int,int>, unsigned> editDistanceCache;
  diffCount[0][0] = 0;
  for (int j = 1; j <= m; ++j) {
    diffCount[0][j] = diffCount[0][j-1] + file2Lines[j-1].length();
  }

  for (int i = 1; i <= n; ++i) {
    diffCount[i][0] = diffCount[i-1][0] + file1Lines[i - 1].length();
  }

  for (int i = 1; i <= n; ++i) {
    for (int j = 1; j <= m; ++j) {
      int distance = editDistance(file1Lines[i-1], file2Lines[j-1]);
      editDistanceCache[std::make_pair(i-1, j-1)] = distance;
      diffCount[i][j] = diffCount[i-1][j-1] + distance;
      unsigned len1 = file1Lines[i-1].length();
      unsigned len2 = file2Lines[i-1].length();
      diffCount[i][j] = std::min(diffCount[i][j], diffCount[i-1][j] + len1);
      diffCount[i][j] = std::min(diffCount[i][j], diffCount[i][j-1] + len2);
    }
  }

  int i = n;
  int j = m;
  std::vector<std::pair<int,lineChangeType>> changes;
  while (i> 0 || j > 0) {
    if (i > 0 && j > 0 && editDistanceCache[{i-1,j-1}] + diffCount[i-1][j-1] == diffCount[i][j]) {
      if (file1Lines[i-1] == file2Lines[j-1]) {
        changes.push_back({i, lineChangeType::same});
      } else {
        changes.push_back({i, lineChangeType::change});
      }
      --i;
      --j;
    } else if ( i > 0 && diffCount[i][j] == diffCount[i-1][j] + file1Lines[i-1].length()) {
      changes.push_back({i, lineChangeType::add});
      --i;
    } else {
      changes.push_back({i, lineChangeType::del});
      --i;
    }
  }
  std::reverse(changes.begin(), changes.end());
  return changes;
}
