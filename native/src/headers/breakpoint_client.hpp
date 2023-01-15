#include <grpcpp/grpcpp.h>
#include <optional>

#include "service.grpc.pb.h"
#include "service.pb.h"

namespace defuzz
{

    class BreakPointClient
    {
    public:
        BreakPointClient(std::shared_ptr<grpc::Channel> channel)
            : stub_(BreakPointAnalyzer::NewStub(channel)) {}

        std::optional<BreakPointResponse> GetBreakPoints(const unsigned char *class_data, size_t size);

    private:
        std::unique_ptr<BreakPointAnalyzer::Stub> stub_;
    };

} // namespace defuzz
