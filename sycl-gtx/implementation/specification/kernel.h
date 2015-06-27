#pragma once

// 3.5.4 Kernel class

#include "command_group.h"
#include "context.h"
#include "error_handler.h"
#include "info.h"
#include "param_traits.h"
#include "program.h"
#include "ranges.h"
#include "refc.h"
#include "../common.h"
#include "../debug.h"
#include <algorithm>

namespace cl {
namespace sycl {

// Forward declarations
class context;
class queue;

namespace detail {
namespace kernel_ {
	class source;
}
}

class kernel {
private:
	detail::refc<cl_kernel, clRetainKernel, clReleaseKernel> kern;
	context ctx;
	program prog;
	detail::command_group cg;

	friend class detail::kernel_::source;

public:
	// The default object is not valid because there is no program or cl_kernel associated with it
	kernel() = delete;
	kernel(nullptr_t) = delete;

	// Constructs from a valid, initialized OpenCL kernel
	kernel(cl_kernel openclKernelObject);

	~kernel();

	// Return the OpenCL kernel object for this kernel.
	cl_kernel get() const {
		return kern.get();
	}

	// Return the context that this kernel is defined for.
	context get_context() const {
		return ctx;
	}

	// Return the program that this kernel is part of.
	program get_program() const {
		return prog;
	}

	template <info::kernel param>
	typename param_traits<info::kernel, param>::type
	get_info() const {
		using return_t = param_traits_t<info::kernel, param>;
		return detail::non_vector_traits<
			info::kernel,
			param,
			detail::traits_buffer_default<return_t>::size
		>().get(kern.get());
	}

	// Return the name of the kernel function
	string_class get_kernel_attributes() const {
		return get_info<info::kernel::attributes>();
	}

	// Return the name of the kernel function
	string_class get_function_name() {
		return get_info<info::kernel::function_name>();
	}

private:
	void enqueue_task(queue* q) const;

	template <int dimensions>
	void enqueue_range(queue* q, range<dimensions> num_work_items, id<dimensions> offset) const {
		size_t* global_work_size = &num_work_items[0];
		size_t* offst = &((size_t&)offset[0]);

		auto error_code = clEnqueueNDRangeKernel(
			q->get(), kern.get(), dimensions,
			offst, global_work_size, nullptr,
			// TODO: Events
			0, nullptr, nullptr
		);
		detail::error::report(error_code);
	}

	template <int dimensions>
	void enqueue_nd_range(queue* q, nd_range<dimensions> execution_range) const {
		size_t* local_work_size = &execution_range.get_local()[0];
		size_t* offst = &((size_t&)execution_range.get_offset()[0]);

		size_t global_work_size[dimensions];
		size_t* start = &execution_range.get_global()[0];
		std::copy(start, start + dimensions, global_work_size);

		// Adjust global work size
		for(int i = 0; i < dimensions; ++i) {
			auto remainder = global_work_size[i] % local_work_size[i];
			if(remainder > 0) {
				global_work_size[i] += local_work_size[i] - remainder;
			}
		}

		auto error_code = clEnqueueNDRangeKernel(
			q->get(), kern.get(), dimensions,
			offst, global_work_size, local_work_size,
			// TODO: Events
			0, nullptr, nullptr
		);
		detail::error::report(error_code);
	}
};

} // namespace sycl
} // namespace cl
