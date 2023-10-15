#include <hwinfo/cus_notifier.h>


int cable_register_client(struct notifier_block *nb)
{
    return blocking_notifier_chain_register(&cable_status_notifier_list, nb);
}
EXPORT_SYMBOL(cable_register_client);
int cable_unregister_client(struct notifier_block *nb)
{
    return blocking_notifier_chain_unregister(&cable_status_notifier_list, nb);
}
EXPORT_SYMBOL(cable_unregister_client);
int cable_notifier_call_chain(unsigned long val, void *v)
{
    return blocking_notifier_call_chain(&cable_status_notifier_list, val, v);
}
EXPORT_SYMBOL(cable_notifier_call_chain);


