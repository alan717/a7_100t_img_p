import redis
import csv


if __name__ == '__main__':
    # provide your redis configuration infromation here
	cfg = { 'host': 'localhost',
	'port': 6379,
	'password': None,
	'ssl': False
	}
	r = redis.StrictRedis(host='localhost', port=6379, db=0)
	
	r.set('foo', 'bar')
	print r.get('foo')
#r.pipeline(transaction=True)
