import React from 'react';
import { Link, useLocation } from 'react-router-dom';
import {
  HomeIcon,
  UserGroupIcon,
  DocumentReportIcon,
  ChartBarIcon,
  CogIcon,
  ShieldCheckIcon,
  FingerPrintIcon,
  QueueListIcon,
} from '@heroicons/react/24/outline';

const Sidebar = () => {
  const location = useLocation();
  
  const menuItems = [
    { name: 'Dashboard', icon: HomeIcon, path: '/' },
    { name: 'Criminal Database', icon: UserGroupIcon, path: '/criminals' },
    { name: 'Fingerprint Registry', icon: FingerPrintIcon, path: '/fingerprints' },
    { name: 'Weapon Registry', icon: ShieldCheckIcon, path: '/weapons' },
    { name: 'Crime Reports', icon: DocumentReportIcon, path: '/reports' },
    { name: 'Queue Management', icon: QueueListIcon, path: '/queue' },
    { name: 'Crime Analytics', icon: ChartBarIcon, path: '/analytics' },
    { name: 'System Settings', icon: CogIcon, path: '/settings' },
  ];

  return (
    <div className="w-64 bg-crime-dark text-white h-screen fixed left-0 top-0 overflow-y-auto">
      <div className="p-6">
        <div className="flex items-center space-x-3">
          <div className="w-10 h-10 bg-crime-primary rounded-lg flex items-center justify-center">
            <ShieldCheckIcon className="w-6 h-6" />
          </div>
          <div>
            <h1 className="text-xl font-bold">Crime Intel</h1>
            <p className="text-xs text-gray-400">Intelligence System</p>
          </div>
        </div>
      </div>
      
      <nav className="mt-8">
        <div className="px-4 mb-4">
          <p className="text-xs font-semibold text-gray-400 uppercase tracking-wider">
            Main Menu
          </p>
        </div>
        
        <ul className="space-y-2 px-4">
          {menuItems.map((item) => {
            const Icon = item.icon;
            const isActive = location.pathname === item.path;
            
            return (
              <li key={item.name}>
                <Link
                  to={item.path}
                  className={`flex items-center space-x-3 px-4 py-3 rounded-lg transition-colors ${
                    isActive
                      ? 'bg-crime-primary text-white'
                      : 'text-gray-300 hover:bg-gray-800 hover:text-white'
                  }`}
                >
                  <Icon className="w-5 h-5" />
                  <span className="font-medium">{item.name}</span>
                </Link>
              </li>
            );
          })}
        </ul>
        
        <div className="absolute bottom-0 w-full p-4 border-t border-gray-800">
          <div className="flex items-center space-x-3">
            <div className="w-8 h-8 bg-crime-primary rounded-full"></div>
            <div>
              <p className="text-sm font-medium">Admin User</p>
              <p className="text-xs text-gray-400">Super Admin</p>
            </div>
          </div>
        </div>
      </nav>
    </div>
  );
};

export default Sidebar;
