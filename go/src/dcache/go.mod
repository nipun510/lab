module lab.com/dcache

go 1.24.6

replace mutility => ../mutility

replace lab.com/mutility => ../mutility

require (
	google.golang.org/grpc v1.75.1
	google.golang.org/protobuf v1.36.9
	lab.com/mutility v0.0.0-00010101000000-000000000000
)

require (
	golang.org/x/net v0.41.0 // indirect
	golang.org/x/sys v0.33.0 // indirect
	golang.org/x/text v0.26.0 // indirect
	google.golang.org/genproto/googleapis/rpc v0.0.0-20250707201910-8d1bb00bc6a7 // indirect
)
