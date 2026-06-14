# Sanitization Report

## Sensitive Data Found
The original RTF code contained a real phone number for GSM SMS/call alerts.

## Action Taken
The real phone number was removed and replaced with:

```text
+XXX-XXXX-XXXX
```

## Files Not Published
No private `.rtf` source is included in the public version. A cleaned `.ino` file is included instead.
