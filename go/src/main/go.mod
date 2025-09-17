module lab.com/main

go 1.24.6

replace lab.com/dcache => ../dcache

replace lab.com/mutility => ../mutility

require lab.com/dcache v0.0.0-00010101000000-000000000000

require lab.com/mutility v0.0.0-00010101000000-000000000000 // indirect
