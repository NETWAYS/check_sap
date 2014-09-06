h1. check_sap


h2. Overview

check_sap connects to CCMS (Computing Center Management System) on a SAP system via the RFC interface.


!CCMS.jpg!

h2. How it works

The plugin connects to an existing CCMS system via TCP/IP using the RFC Interface.
It then establishes an XMI (external Monitoring Interface) session,
calls various XAL (external Interface for Alert Management) functions to retrieve monitoring objects,
attempts to match these objects against the requested pattern,
fetches any applicable status / performance data,
ends the XMI session,
and disconnects from CCMS.

Finally it outputs the data in a format suitable for nagios.





h2. Features

* fast
* resource friendly (C Code)
* can connect to unicode SAP-Systems
* compiles against NetWeaver or the Classic SDK
* runs on 64-bit Linux
* simple command-line configuration
* multi-line output (Nagios 3 compatible)
* full-path matching capability
* Graphable performance data
* list mode allows easy searching for monitoring objects



h2. Installation


h3. Requirements


Compilation has only been tested on linux systems, but should work on other
systems with a suitable SDK from SAP.

You need to have the following;
* autoconf,
* development packages gcc, make etc.,
* the RFC SDK from SAP (the plugin has been tested with version 7.0 Unicode - 64bit and with NetWeaver)
which is available from http://service.sap.com,
* an installed and configured CCMS,
* a suitable RFC User for the CCMS



h3. Compilation

Read the README file first!

Download and unpack the SDK

1. Copy the libraries from _lib_ to _/usr/local/lib_
2. Run *ldconfig* to rebuild the library cache
3. Check that you are not missing any system libraries
<pre>
/usr/local/lib# ldd librfcum.so 
    linux-vdso.so.1 =>  (0x00007fff29921000)
    libdl.so.2 => /lib/libdl.so.2 (0x00007f814b9c7000)
    librt.so.1 => /lib/librt.so.1 (0x00007f814b7be000)
    libpthread.so.0 => /lib/libpthread.so.0 (0x00007f814b5a1000)
    libc.so.6 => /lib/libc.so.6 (0x00007f814b24e000)
    libstdc++.so.5 => /usr/lib/libstdc++.so.5 (0x00007f814af72000)
    libm.so.6 => /lib/libm.so.6 (0x00007f814acee000)
    libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007f814aad7000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f814c23c000)
/usr/local/lib# ldd libsapu16.so 
    linux-vdso.so.1 =>  (0x00007fff481bc000)
    libdl.so.2 => /lib/libdl.so.2 (0x00007f6854909000)
    librt.so.1 => /lib/librt.so.1 (0x00007f6854700000)
    libc.so.6 => /lib/libc.so.6 (0x00007f68543ac000)
    libstdc++.so.5 => /usr/lib/libstdc++.so.5 (0x00007f68540d0000)
    libm.so.6 => /lib/libm.so.6 (0x00007f6853e4d000)
    libgcc_s.so.1 => /lib/libgcc_s.so.1 (0x00007f6853c35000)
    libpthread.so.0 => /lib/libpthread.so.0 (0x00007f6853a19000)
    /lib64/ld-linux-x86-64.so.2 (0x00007f6854c53000)
</pre>
if anything is missing ("not found" appears) then install it with your system's package
manager.

4. build the configure script with *autoconf*
5. build the Makefile with *./configure --with-rfc-headers=/path/to/include --with-rfc-libs=/usr/local/lib*
<pre>
checking for gcc... gcc
checking for C compiler default output file name... a.out
checking whether the C compiler works... yes
checking whether we are cross compiling... no
checking for suffix of executables... 
checking for suffix of object files... o
checking whether we are using the GNU C compiler... yes
checking whether gcc accepts -g... yes
checking for gcc option to accept ISO C89... none needed
checking for a BSD-compatible install... /usr/bin/install -c
checking how to run the C preprocessor... gcc -E
checking for grep that handles long lines and -e... /bin/grep
checking for egrep... /bin/grep -E
checking for ANSI C header files... yes
checking for sys/types.h... yes
checking for sys/stat.h... yes
checking for stdlib.h... yes
checking for string.h... yes
checking for memory.h... yes
checking for strings.h... yes
checking for inttypes.h... yes
checking for stdint.h... yes
checking for unistd.h... yes
checking for wchar_t... yes
checking size of wchar_t... 4
configure: WARNING: Your wchar_t is 4 bytes long - enabling workarounds
checking saprfc.h usability... yes
checking saprfc.h presence... yes
checking for saprfc.h... yes
checking for RfcOpenExU in -lrfcum... yes
configure: creating ./config.status
config.status: creating Makefile
</pre>

Confirm that everything is OK (you can safely ignore the wchar_t warning
because the plugin contains a workaround for it - see the source code for
a more detailed discussion of the WCHAR problem)

Additional options are available with *configure --help*

6. build the plugin itself with *make*
7. (optionally) install the plugin to _/usr/local/nagios/libexec/_ with *make install*


h3. SAP System configuration

In order to access the CCMS you will require a valid RFC user and
suitable access rights.

Create a user (e.g. nagios) and assign the following role to it:





|_. Option |_. Value |
|\2=. S_RFC - Authorization check for RFC access |
| Activity | * |
| Name of RFC to be protected | SALX,SXHC,SXMI,SYST |
| Type of RFC object to be protected | FUGR |
| |
|\2=. S_XMI_LOG - Internal access authorization for XMI log |
| Access method for XMI log | * |
| |
|\2=. S_XMI_PROD - Auth. for external management interfaces (XMI) |
| XMI logging: company name | NETWAYS |
| Product | CHECK_CCMS |
| Interface ID | * |


(or for German language systems)
|_. Option |_. Wert |
|\2=. S_RFC - Berechtigungsprüfung beim RFC-Zugriff |
| Aktivität | * |
| Name des zu schützenden RFC-Obj | SALX,SXHC,SXMI,SYST |
| Typ des zu schützenden RFC-Obj | FUGR |
| |
|\2=. S_XMI_LOG - Interne Zugriffsberechtigung für XMI-Log |
| Zugriffsmethode für XMI-Log | * |
| |
|\2=. S_XMI_PROD - Berechtigung für Externe Management-Schnittstelle (XMI) |
| XMI-Logging: Firmenname | NETWAYS |
| XMI-Logging: Programmname | CHECK_CCMS |
| Kürzel für das Interface | * |


h3. Testing

You can test the access by calling the plugin in "sapinfo" mode, and it should return the system
version.
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios --mode sapinfo
Connected to SAP release 701 
</pre>




h2. Usage



img CCMS.jpg

h3. Options

The important options are;

 -dest          destination (System ID; default is SAP)
 -client        client (defaults to 000)
 -user          RFC-User (default is user)
 -pass          Password for this user (default is pass)
 -lang          language (defauts to E)
 -3             use version 3 (default)
 -trace         create .trc files in the working directory for debugging
 -host          IP address of server
 -sysnr         SysNr (default is 00)
 -fullpath      Outputs the complete path of an object, with / as a separator (useful if you want to match against the path)
 --trim         Remove this number of leading parts from the performance data (useful with fullpath)
 -mode          the operation mode of the plugin, being one of;

    sapinfo         prints basic system information
    listsets        lists all available monitor sets
    listmonitors    list all monitors in a given set (requires 1 parameter)
    listtree        lists all indicators in a given set + monitor (requires 2 parameters)



h3. Examples

List all monitor sets
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios --mode listsets
SAP CCMS Web Admin Monitor Templates
SAP CCMS Technical Operations Templates
SAP CCMS Technical Expert Monitors
SAP CCMS Monitors for Optional Components
SAP CCMS Monitor Templates
SAP CCMS Admin Workplace
...
</pre>

List all monitors for a set
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios --mode listmonitors 'SAP CCMS Monitor Templates'
SAP CCMS Monitor Templates / System Errors
SAP CCMS Monitor Templates / System Configuration
SAP CCMS Monitor Templates / Syslog
SAP CCMS Monitor Templates / Spool System
SAP CCMS Monitor Templates / Security
SAP CCMS Monitor Templates / Performance Overview
SAP CCMS Monitor Templates / Operating System
SAP CCMS Monitor Templates / Filesystems
SAP CCMS Monitor Templates / Entire System
SAP CCMS Monitor Templates / Dialog per Application Server
SAP CCMS Monitor Templates / Database
SAP CCMS Monitor Templates / Dialog Overview
...
</pre>

List all counters in a monitor
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios --mode listtree 'SAP CCMS Monitor Templates' 'Entire System'
  Entire System                           
    SID                                     
      Application Server                      
        R3App
          OperatingSystem                         
...
          DatabaseClient                          
...
          R3Services                              
            Dialog                                  
              ResponseTime                            
              FrontendResponseTime                    
              QueueTime                               
              Load+GenTime                            
              RollTime                                
              DBRequestTime                           
              Utilisation                             
              PrivMode Utilisation                    
              NumberOfWpDIA                           
              ErrorsInWpDIA                           
              ErrorFreqInWpDIA                        
              EndedWpDIA                              
              QueueLength                             
              LongRunners                             
              ProgramErrors                           
              DialogSteps                             
              GuiCallbackTime                         
              FrontEndNetTime                         
              MonitoringTime                          
              ResponseTime(StandardTran.)             
              LogonLoadQuality                        
              LogonLoadMessage                        
              UsersLoggedIn                           
</pre>

List individual counters
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios 'SAP CCMS Monitor Templates' 'Entire System' 'ResponseTime'
[OK] 8 matching lines found | ResponseTime=193msec FrontendResponseTime=445msec ResponseTime(StandardTran.)=9msec ResponseTime=943msec ResponseTime=175msec ResponseTime=193msec FrontendResponseTime=445msec ResponseTime(StandardTran.)=9msec 
[OK] SID/ResponseTime: 193 msec
[OK] SID/FrontendResponseTime: 445 msec
[OK] SID/ResponseTime(StandardTran.): 9 msec
[OK] SID/ResponseTime: 943 msec
[OK] SID/ResponseTime: 175 msec
[OK] SID/ResponseTime: 193 msec
[OK] SID/FrontendResponseTime: 445 msec
[OK] SID/ResponseTime(StandardTran.): 9 msec
</pre>


List individual counters with full path
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios 'SAP CCMS Monitor Templates' 'Entire System' 'ResponseTime' -f
[OK] 8 matching lines found | Entire_System-SID-Application_Server-R3App-R3Services-Dialog-ResponseTime=193msec Entire_System-SID-Application_Server-R3App-R3Services-Dialog-FrontendResponseTime=445msec Entire_System-SID-Application_Server-R3App-R3Services-Dialog-ResponseTime(StandardTran.)=9msec Entire_System-SID-Application_Server-R3App-R3Services-Update-PerformanceU1-ResponseTime=943msec Entire_System-SID-Application_Server-R3App-R3Services-Update-PerformanceU2-ResponseTime=175msec Entire_System-SID-R-3_Services-Dialog-Dialog-ResponseTime=193msec Entire_System-SID-R-3_Services-Dialog-Dialog-FrontendResponseTime=445msec Entire_System-SID-R-3_Services-Dialog-Dialog-ResponseTime(StandardTran.)=9msec 
[OK] SID/Entire System/SID/Application Server/R3App/R3Services/Dialog/ResponseTime: 193 msec
[OK] SID/Entire System/SID/Application Server/R3App/R3Services/Dialog/FrontendResponseTime: 445 msec
[OK] SID/Entire System/SID/Application Server/R3App/R3Services/Dialog/ResponseTime(StandardTran.): 9 msec
[OK] SID/Entire System/SID/Application Server/R3App/R3Services/Update/PerformanceU1/ResponseTime: 943 msec
[OK] SID/Entire System/SID/Application Server/R3App/R3Services/Update/PerformanceU2/ResponseTime: 175 msec
[OK] SID/Entire System/SID/R/3 Services/Dialog/Dialog/ResponseTime: 193 msec
[OK] SID/Entire System/SID/R/3 Services/Dialog/Dialog/FrontendResponseTime: 445 msec
[OK] SID/Entire System/SID/R/3 Services/Dialog/Dialog/ResponseTime(StandardTran.): 9 msec
</pre>


Trim the performance data for PNP
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios 'SAP CCMS Monitor Templates' 'Database' 'Used space' -f --trim 4
[OK] 7 matching lines found | PSAPAPP-Used_space=22% PSAPAPPUSR-Used_space=0% PSAPTEMP-Used_space=0% PSAPUNDO-Used_space=0% SYSAUX-Used_space=8% SYSTEM-Used_space=9% PSAPAPP702-Used_space=32% 
[OK] SAP/Database/Oracle/Space management/Tablespaces/PSAPAPP/Used space: 22 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/PSAPAPPUSR/Used space: 0 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/PSAPTEMP/Used space: 0 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/PSAPUNDO/Used space: 0 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/SYSAUX/Used space: 8 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/SYSTEM/Used space: 9 %
[OK] SAP/Database/Oracle/Space management/Tablespaces/PSAPAPP702/Used space: 32 %
</pre>

Restrict the output to a single counter
<pre>
~> check_sap --host saphost --sysnr 00 --client 000 --dest SID --user NAGIOS --pass nagios 'SAP CCMS Monitor Templates' 'Entire System' 'Dialog/Dialog/FrontendResponseTime' -f
[OK] SID/Entire System/SID/R/3 Services/Dialog/Dialog/FrontendResponseTime: 445 msec | Entire_System-SID-R-3_Services-Dialog-Dialog-FrontendResponseTime=445msec
</pre>



h3. Thresholds


The thresholds are set in the SAP system; the status is passed directly to nagios


h3. Integration with Nagios


The following sample configurations should help you get started

Command definition
<pre>
define command{
  command_name      check_sap
  command_line      $USER1$/check_sap --host $HOSTADDRESS$ --sysnr 00 --client 000 --dest $ARG1$ --user $USER4$ --pass $USER5$ '$ARG2$' '$ARG3$' '$ARG4'
}
</pre>


Service Definition
<pre>
define service{
    hostname            saphost
    service_description CCMS-Dialog Response Time
    check_command       check_sap!SID!SAP CCMS Monitor Templates!Dialog Overview!ResponseTime$$
}
</pre>


h3. Security Issues

Connection details are passed to the plugin as command-line parameters, which can be seen by other
users of the system in the processlist - but you aren't running your monitoring on a shared host anyway
are you?



h3. FAQ

Q. I get the message 'Error compiling regex'
A. You need to specify the filter as a standard regular expression

Q. How do I match at the beginning / end of a line?
A. with ^ and $

Q. Using the $ sign works on the command line but not in Nagios
A. Replace $ with $$ in the configuration

Q. I get 'Error: (RFC_ERROR_LOGON_FAILURE) Name or password is incorrect (repeat logon)'
A. The login details are incorrect or there are insufficient rights - try the username in uppercase

Q. My filter is matching too many counters
A. Use the -f option to match against a full path

Q. The performance data labels are too long for my grapher
A. Use the --trim option to trim the path

h2. Disclaimer

This program is designed for interoperability between SAP&reg; CCMS and Nagios&reg; or compatible monitoring software.
SAP Trademarks are owned by SAP AG, Nagios trademarks by Nagios Enterprises LLC.
