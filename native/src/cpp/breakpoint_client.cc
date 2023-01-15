#include "breakpoint_client.hpp"

#include <condition_variable>
#include <mutex>

namespace defuzz
{
    std::optional<BreakPointResponse> BreakPointClient::GetBreakPoints(const unsigned char *class_data, size_t size)
    {
        BreakPointRequest request;
        request.set_classdata(class_data, size);
        grpc::ClientContext context;

        BreakPointResponse response;

        std::mutex mu;
        std::condition_variable cv;
        bool done = false;
        grpc::Status status;
        stub_->async()->GetBreakPoints(&context, &request, &response,
                                       [&mu, &cv, &done, &status](grpc::Status s)
                                       {
                                           std::unique_lock<std::mutex> lock(mu);
                                           status = std::move(s);
                                           done = true;
                                           cv.notify_one();
                                       });
        std::unique_lock<std::mutex> lock(mu);
        while (!done)
        {
            cv.wait(lock);
        }

        if (status.ok())
        {
            return response;
        }
        else
        {
            return {};
        }
    }

} // namespace defuzz
