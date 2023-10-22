package com.unionpay.client3.tsm;

import android.os.Parcel;
import android.os.Parcelable;

public class SeAppInfo implements Parcelable {

    private String mAppAid;
    private String mAppIcon;
    private String mAppName;
    private String mBankId;
    private String mAppProviderName;
    private String mAppProviderLogo;
    private String mAppSummary;
    private String mAppVersion;
    private int mDownloadTimes;

    private String mPan;
    private String mValidDate;
    private String mBalance;
    private String mCVN2;

    public static final Creator<SeAppInfo> CREATOR = new Creator<SeAppInfo>() {

        @Override
        public SeAppInfo createFromParcel(Parcel source) {
            return new SeAppInfo(source);
        }

        @Override
        public SeAppInfo[] newArray(int size) {
            return new SeAppInfo[size];
        }

    };

    public SeAppInfo() {

    }

    public SeAppInfo(Parcel source) {
        readFromParcel(source);
    }

    @Override
    public int describeContents() {
        return 0;
    }

    @Override
    public void writeToParcel(Parcel dest, int flags) {
        dest.writeString(mAppAid);
        dest.writeString(mAppIcon);
        dest.writeString(mAppName);
        dest.writeString(mBankId);
        dest.writeString(mAppProviderName);
        dest.writeString(mAppProviderLogo);
        dest.writeString(mAppSummary);
        dest.writeString(mAppVersion);
        dest.writeInt(mDownloadTimes);

        dest.writeString(mPan);
        dest.writeString(mValidDate);
        dest.writeString(mBalance);
        dest.writeString(mCVN2);
    }

    public void readFromParcel(Parcel source) {
        mAppAid = source.readString();
        mAppIcon = source.readString();
        mAppName = source.readString();
        mBankId = source.readString();
        mAppProviderName = source.readString();
        mAppProviderLogo = source.readString();
        mAppSummary = source.readString();
        mAppVersion = source.readString();
        mDownloadTimes = source.readInt();

        mPan = source.readString();
        mValidDate = source.readString();
        mBalance = source.readString();
        mCVN2 = source.readString();
    }

    public String getAppAid() {
        return mAppAid;
    }

    public void setAppAid(String appAid) {
        mAppAid = appAid;
    }

    public String getAppIcon() {
        return mAppIcon;
    }

    public void setAppIcon(String appIcon) {
        mAppIcon = appIcon;
    }

    public String getAppName() {
        return mAppName;
    }

    public void setAppName(String appName) {
        mAppName = appName;
    }

    public String getBankId() {
        return mBankId;
    }

    public void setBankId(String bankId) {
        mBankId = bankId;
    }

    public String getAppProviderName() {
        return mAppProviderName;
    }

    public void setAppProviderName(String appProviderName) {
        mAppProviderName = appProviderName;
    }

    public String getAppProviderLogo() {
        return mAppProviderLogo;
    }

    public void setAppProviderLogo(String appProviderLogo) {
        mAppProviderLogo = appProviderLogo;
    }

    public String getAppSummary() {
        return mAppSummary;
    }

    public void setAppSummary(String appSummary) {
        mAppSummary = appSummary;
    }

    public String getAppVersion() {
        return mAppVersion;
    }

    public void setAppVersion(String appVersion) {
        mAppVersion = appVersion;
    }

    public int getDownloadTimes() {
        return mDownloadTimes;
    }

    public void setDownloadTimes(int downloadTimes) {
        mDownloadTimes = downloadTimes;
    }

    public String getPan() {
        return mPan;
    }

    public void setPan(String pan) {
        mPan = pan;
    }

    public String getValidDate() {
        return mValidDate;
    }

    public void setValidDate(String validDate) {
        mValidDate = validDate;
    }

    public String getBalance() {
        return mBalance;
    }

    public void setBalance(String balance) {
        mBalance = balance;
    }

    public String getCVN2() {
        return mCVN2;
    }

    public void setCVN2(String cvn2) {
        mCVN2 = cvn2;
    }
}
