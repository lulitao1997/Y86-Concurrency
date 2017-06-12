if (strcmp(uri, "/next") == 0) {
    if (!ended) {
        before_wake_thread();
        f_lock.unlock();
        d_lock.unlock();
        e_lock.unlock();
        m_lock.unlock(); /*this wake those echo threads*/
        w_lock.unlock(); /*this wake those echo threads*/
        cerr << "unlocked!" << endl;
        fin_lock.lock(); /*wait echo threads to finish*/
        cerr << "threads finished" << endl;
        PIPE[clock_cnt].stat = PIPE[clock_cnt].W.stat;
    }
}