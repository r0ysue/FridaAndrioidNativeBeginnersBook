/*
 * This file is auto-generated.  DO NOT MODIFY.
 * Original file: E:\\MyUPCode\\UPClient_3_0_lib\\src\\com\\unionpay\\client3\\tsm\\ITsmConnection.aidl
 */
package com.unionpay.client3.tsm;
public interface ITsmConnection extends android.os.IInterface
{
/** Local-side IPC implementation stub class. */
public static abstract class Stub extends android.os.Binder implements com.unionpay.client3.tsm.ITsmConnection
{
private static final java.lang.String DESCRIPTOR = "com.unionpay.client3.tsm.ITsmConnection";
/** Construct the stub at attach it to the interface. */
public Stub()
{
this.attachInterface(this, DESCRIPTOR);
}
/**
 * Cast an IBinder object into an com.unionpay.client3.tsm.ITsmConnection interface,
 * generating a proxy if needed.
 */
public static com.unionpay.client3.tsm.ITsmConnection asInterface(android.os.IBinder obj)
{
if ((obj==null)) {
return null;
}
android.os.IInterface iin = obj.queryLocalInterface(DESCRIPTOR);
if (((iin!=null)&&(iin instanceof com.unionpay.client3.tsm.ITsmConnection))) {
return ((com.unionpay.client3.tsm.ITsmConnection)iin);
}
return new com.unionpay.client3.tsm.ITsmConnection.Stub.Proxy(obj);
}
@Override public android.os.IBinder asBinder()
{
return this;
}
@Override public boolean onTransact(int code, android.os.Parcel data, android.os.Parcel reply, int flags) throws android.os.RemoteException
{
switch (code)
{
case INTERFACE_TRANSACTION:
{
reply.writeString(DESCRIPTOR);
return true;
}
case TRANSACTION_sendApdu:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
java.lang.String _arg1;
_arg1 = data.readString();
int _arg2;
_arg2 = data.readInt();
java.lang.String _result = this.sendApdu(_arg0, _arg1, _arg2);
reply.writeNoException();
reply.writeString(_result);
return true;
}
case TRANSACTION_closeChannel:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
int _arg1;
_arg1 = data.readInt();
this.closeChannel(_arg0, _arg1);
reply.writeNoException();
return true;
}
case TRANSACTION_getSeApps:
{
data.enforceInterface(DESCRIPTOR);
int _arg0;
_arg0 = data.readInt();
com.unionpay.client3.tsm.SeAppInfo[] _result = this.getSeApps(_arg0);
reply.writeNoException();
reply.writeTypedArray(_result, android.os.Parcelable.PARCELABLE_WRITE_RETURN_VALUE);
return true;
}
}
return super.onTransact(code, data, reply, flags);
}
private static class Proxy implements com.unionpay.client3.tsm.ITsmConnection
{
private android.os.IBinder mRemote;
Proxy(android.os.IBinder remote)
{
mRemote = remote;
}
@Override public android.os.IBinder asBinder()
{
return mRemote;
}
public java.lang.String getInterfaceDescriptor()
{
return DESCRIPTOR;
}
@Override public java.lang.String sendApdu(int seType, java.lang.String hexApdu, int channel) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
java.lang.String _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(seType);
_data.writeString(hexApdu);
_data.writeInt(channel);
mRemote.transact(Stub.TRANSACTION_sendApdu, _data, _reply, 0);
_reply.readException();
_result = _reply.readString();
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
@Override public void closeChannel(int seType, int channel) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(seType);
_data.writeInt(channel);
mRemote.transact(Stub.TRANSACTION_closeChannel, _data, _reply, 0);
_reply.readException();
}
finally {
_reply.recycle();
_data.recycle();
}
}
@Override public com.unionpay.client3.tsm.SeAppInfo[] getSeApps(int seType) throws android.os.RemoteException
{
android.os.Parcel _data = android.os.Parcel.obtain();
android.os.Parcel _reply = android.os.Parcel.obtain();
com.unionpay.client3.tsm.SeAppInfo[] _result;
try {
_data.writeInterfaceToken(DESCRIPTOR);
_data.writeInt(seType);
mRemote.transact(Stub.TRANSACTION_getSeApps, _data, _reply, 0);
_reply.readException();
_result = _reply.createTypedArray(com.unionpay.client3.tsm.SeAppInfo.CREATOR);
}
finally {
_reply.recycle();
_data.recycle();
}
return _result;
}
}
static final int TRANSACTION_sendApdu = (android.os.IBinder.FIRST_CALL_TRANSACTION + 0);
static final int TRANSACTION_closeChannel = (android.os.IBinder.FIRST_CALL_TRANSACTION + 1);
static final int TRANSACTION_getSeApps = (android.os.IBinder.FIRST_CALL_TRANSACTION + 2);
}
public java.lang.String sendApdu(int seType, java.lang.String hexApdu, int channel) throws android.os.RemoteException;
public void closeChannel(int seType, int channel) throws android.os.RemoteException;
public com.unionpay.client3.tsm.SeAppInfo[] getSeApps(int seType) throws android.os.RemoteException;
}
