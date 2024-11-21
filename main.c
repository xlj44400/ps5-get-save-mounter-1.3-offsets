#include <stdio.h>
#include <stdarg.h>

#include <ps5/kernel.h>

// https://github.com/ps5-payload-dev/sdk/blob/master/crt/rtld_sprx.c
static int (*sceKernelLoadStartModule)(const char *, unsigned long, const void *,
                                       unsigned int, void *, int *) = 0;
int (*sceSysmoduleLoadModuleInternal)(unsigned int) = 0;

typedef struct notify_request
{
    char useless1[45];
    char message[3075];
} notify_request_t;

int sceKernelSendNotificationRequest(int, notify_request_t *, size_t, int);

void notifyf(const char *fmt, ...)
{
    notify_request_t req = {0};
    va_list args;
    va_start(args, fmt);
    vsnprintf(req.message, sizeof(req.message), fmt, args);
    va_end(args);
    sceKernelSendNotificationRequest(0, &req, sizeof(req), 0);
}

int main()
{
    // Setup
    unsigned int libkernel = 0x1;
    if (!kernel_dynlib_dlsym(-1, libkernel, "sceKernelDlsym"))
    {
        libkernel = 0x2001;
    }

    sceKernelLoadStartModule = (void *)kernel_dynlib_dlsym(-1, libkernel, "sceKernelLoadStartModule");
    if (!sceKernelLoadStartModule)
    {
        notifyf("Failed to get sceKernelLoadStartModule");
        return -1;
    }

    uint32_t libSceSysmodule_handle = 0;
    if (kernel_dynlib_handle(-1, "libSceSysmodule.sprx", &libSceSysmodule_handle))
    {
        if ((libSceSysmodule_handle = sceKernelLoadStartModule("/system/common/lib/libSceSysmodule.sprx",
                                                               0, 0, 0, 0, 0)) <= 0)
        {
            notifyf("Failed to load libSceSysmodule");
            return -1;
        }
    }

    sceSysmoduleLoadModuleInternal = (void *)kernel_dynlib_dlsym(-1, libSceSysmodule_handle, "sceSysmoduleLoadModuleInternal");
    if (!sceSysmoduleLoadModuleInternal)
    {
        notifyf("Failed to get sceSysmoduleLoadModuleInternal");
        return -1;
    }

    // libSceUserService offsets

    sceSysmoduleLoadModuleInternal(0x80000011);
    uint32_t libSceUserService_handle = 0;
    kernel_dynlib_handle(-1, "libSceUserService.sprx", &libSceUserService_handle);
    if (libSceUserService_handle <= 0)
    {
        notifyf("Failed to load libSceUserService");
        return -1;
    }

    intptr_t libSceUserService_base = kernel_dynlib_entry_addr(-1, libSceUserService_handle);
    if (libSceUserService_base < 0)
    {
        notifyf("Failed to get libSceUserService_base");
        return -1;
    }

    // sceUserServiceGetInitialUser
    // sceUserServiceGetLoginUserIdList
    // sceUserServiceGetUserName
    intptr_t sceUserServiceGetInitialUser_addr = kernel_dynlib_dlsym(-1, libSceUserService_handle, "sceUserServiceGetInitialUser");
    if (sceUserServiceGetInitialUser_addr < 0)
    {
        notifyf("Failed to get sceUserServiceGetInitialUser_addr");
        return -1;
    }

    intptr_t sceUserServiceGetLoginUserIdList_addr = kernel_dynlib_dlsym(-1, libSceUserService_handle, "sceUserServiceGetLoginUserIdList");
    if (sceUserServiceGetLoginUserIdList_addr < 0)
    {
        notifyf("Failed to get sceUserServiceGetLoginUserIdList_addr");
        return -1;
    }

    intptr_t sceUserServiceGetUserName_addr = kernel_dynlib_dlsym(-1, libSceUserService_handle, "sceUserServiceGetUserName");
    if (sceUserServiceGetUserName_addr < 0)
    {
        notifyf("Failed to get sceUserServiceGetUserName_addr");
        return -1;
    }

    notifyf("sceUserServiceGetInitialUser: 0x%X\nsceUserServiceGetLoginUserIdList: 0x%X\nsceUserServiceGetUserName: 0x%X",
        (unsigned int)(sceUserServiceGetInitialUser_addr - libSceUserService_base),
        (unsigned int)(sceUserServiceGetLoginUserIdList_addr - libSceUserService_base),
        (unsigned int)(sceUserServiceGetUserName_addr - libSceUserService_base));
    

    // libSceSaveData offsets

    sceSysmoduleLoadModuleInternal(0x8000000F);
    uint32_t libSceSaveData_handle = 0;
    kernel_dynlib_handle(-1, "libSceSaveData.sprx", &libSceSaveData_handle);
    if (libSceSaveData_handle <= 0)
    {
        notifyf("Failed to load libSceSaveData");
        return -1;
    }

    intptr_t libSceSaveData_base = kernel_dynlib_entry_addr(-1, libSceSaveData_handle);
    if (libSceSaveData_base < 0)
    {
        notifyf("Failed to get libSceSaveData_base");
        return -1;
    }

    // sceSaveDataMount2
    // sceSaveDataUmount
    // sceSaveDataDirNameSearch
    // sceSaveDataTransferringMount
    // sceSaveDataInitialize3
    intptr_t sceSaveDataMount2_addr = kernel_dynlib_dlsym(-1, libSceSaveData_handle, "sceSaveDataMount2");
    if (sceSaveDataMount2_addr < 0)
    {
        notifyf("Failed to get sceSaveDataMount2_addr");
        return -1;
    }

    intptr_t sceSaveDataUmount_addr = kernel_dynlib_dlsym(-1, libSceSaveData_handle, "sceSaveDataUmount");
    if (sceSaveDataUmount_addr < 0)
    {
        notifyf("Failed to get sceSaveDataUmount_addr");
        return -1;
    }

    intptr_t sceSaveDataDirNameSearch_addr = kernel_dynlib_dlsym(-1, libSceSaveData_handle, "sceSaveDataDirNameSearch");
    if (sceSaveDataDirNameSearch_addr < 0)
    {
        notifyf("Failed to get sceSaveDataDirNameSearch_addr");
        return -1;
    }

    intptr_t sceSaveDataTransferringMount_addr = kernel_dynlib_dlsym(-1, libSceSaveData_handle, "sceSaveDataTransferringMount");
    if (sceSaveDataTransferringMount_addr < 0)
    {
        notifyf("Failed to get sceSaveDataTransferringMount_addr");
        return -1;
    }

    intptr_t sceSaveDataInitialize3_addr = kernel_dynlib_dlsym(-1, libSceSaveData_handle, "sceSaveDataInitialize3");
    if (sceSaveDataInitialize3_addr < 0)
    {
        notifyf("Failed to get sceSaveDataInitialize3_addr");
        return -1;
    }

    notifyf("sceSaveDataMount2: 0x%X\nsceSaveDataUmount: 0x%X\nsceSaveDataDirNameSearch: 0x%X\nsceSaveDataTransferringMount: 0x%X\nsceSaveDataInitialize3: 0x%X",
        (unsigned int)(sceSaveDataMount2_addr - libSceSaveData_base),
        (unsigned int)(sceSaveDataUmount_addr - libSceSaveData_base),
        (unsigned int)(sceSaveDataDirNameSearch_addr - libSceSaveData_base),
        (unsigned int)(sceSaveDataTransferringMount_addr - libSceSaveData_base),
        (unsigned int)(sceSaveDataInitialize3_addr - libSceSaveData_base));

    return 0;
}