import requests
import requests

headers = {
    'sec-ch-ua': '^\\^Chromium^\\^;v=^\\^92^\\^, ^\\^',
    'Referer': 'https://news.ycombinator.com/',
    'DNT': '1',
    'sec-ch-ua-mobile': '?0',
    'User-Agent': 'Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/92.0.4515.159 Safari/537.36',
}

params = (
    ('bT7Yn0UZ0XLIbn8300Qc', ''),
)

response = requests.get('https://news.ycombinator.com/news.css', headers=headers, params=params)

#NB. Original query string below. It seems impossible to parse and
#reproduce query strings 100% accurately so the one below is given
#in case the reproduced version is not "correct".
response = requests.get('https://news.ycombinator.com/news.css?bT7Yn0UZ0XLIbn8300Qc', headers=headers)
print(response.text)
