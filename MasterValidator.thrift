include "SharedService.thrift"


//declare languages iface generation
namespace cpp MasterValidation

// declare data structures




// declare services used by workers and clients

service MasterValidation extends SharedService.SharedService{
	void validateBlock(1: SharedService.Block block);
	void validateBlockWithInfo(1: SharedService.Block block);
}
