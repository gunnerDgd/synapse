import dns.resolver
import time

start = time.clock()

my_resolver = dns.resolver.Resolver()

# 8.8.8.8 is Google's public DNS server
my_resolver.nameservers = ['219.250.36.130']

answer = my_resolver.query('www.daum.net')

end = time.clock()
print(end - start)

print(answer.rrset)