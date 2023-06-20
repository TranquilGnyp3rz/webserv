import requests

# URL of the server endpoint to send the POST request to
url = 'http://localhost:7000/api/endpoint'

# Payload (data) to include in the request body
data = {
    'key1': 'value1',
    'key2': 'value2'
}

# Send the POST request
response = requests.post(url, json=data)

# Check the response status code and content
if response.status_code == 200:
    print('POST request was successful.')
    print('Response:', response.json())
else:
    print('POST request failed. Status code:', response.status_code)
