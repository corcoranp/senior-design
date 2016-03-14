

#include "../enums.h"


namespace blaze {


	class port {
	public:
		port(PortConfig portConfig);
		virtual ~port();

		PortConfig portcfg;


	};
}
