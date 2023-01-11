#include <scrat/io/listener.h>

namespace scrat::io
{

listener_base::listener_base(object *parent_)
{
    _ifds.push_back({});
}

listener_base::~listener_base()
{
    //...
    _ifds.clear();
    //...
}

result<> scrat::io::listener_base::listen()
{
    rem::push_debug(source_pffl) < " intentionally call the delegates:";
    on_read_ready(_ifds.back());
    on_write_ready(_ifds.back());
    return rem::notimplemented;
}

result<> listener_base::add_ifd(int fd_, ifd::opt &&opt_) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::remove_ifd(int fd_) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::pause_ifd(int fd_) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::init() {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::shutdown() {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::on_read_ready(ifd &) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::on_write_ready(ifd &) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}

result<> listener_base::on_idle(ifd &) {
    return rem::push_exception(source_ffl) < rem::notimplemented < " cannot be used yet...";
}


}